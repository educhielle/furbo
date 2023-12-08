#pragma once

#include "math.h"
#include "numpy.h"

#ifndef DEFAULT_DEPTH
    #define DEFAULT_DEPTH 3
#endif

namespace smart
{

template <class Ciphertext>
std::vector<int> Stateful<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
int Stateful<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
int Stateful<Ciphertext>::depth = DEFAULT_DEPTH;

template <class Ciphertext>
std::shared_ptr<Ciphertext> Stateful<Ciphertext>::default_zero;

template <class Ciphertext>
Stateful<Ciphertext>::Stateful(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
{
    x = ct;
    t = mod;
    p_zero = zero;
    pool[0] = zero;
    pool[1] = std::make_shared<Ciphertext>(x);
}

template <class Ciphertext>
inline Stateful<Ciphertext> & Stateful<Ciphertext>::calc(const Stateful<Ciphertext> & a, Operation op)
{
    switch (op)
    {
        case ADD : x += a.x; break;
        case MUL : x *= a.x; break;
        case SUB : x -= a.x; break;
        default : throw "Operator not supported";
    }
    counters[op]++;
    return *this;
}

template <class Ciphertext>
inline Stateful<Ciphertext> & Stateful<Ciphertext>::calc(int a, Operation op)
{
    math::reduce(a, t);
    switch (op)
    {
        case ADD : x += a;              break;
        case MUL : x  = mulsaddfull(a); break;
        case SUB : x -= a;              break;
        default : throw "Operator not supported";
    }
    if (op != MUL) { counters[N_OPERATIONS + op]++; }
    return *this;
}

template <class Ciphertext>
bool Stateful<Ciphertext>::increment(std::pair<std::vector<int>,std::vector<int>> & idx, const std::vector<int> & vidx, int half)
{
    int pivot = idx.first.size() - 1;
    while (++idx.second[pivot] == vidx.size())
    {
        if (!pivot) return false;
        pivot--;
    }
    auto ref = idx.second[pivot];
    for (int i=pivot; i < idx.first.size(); i++)
    {
        idx.first[i] = vidx[ref];
        idx.second[i] = ref;
    }
    return true;
}

// computes result recursively by calculating lesser
// coefficients then adding them.
template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::muladd(const Ciphertext & x, int k, int & i)
{
    if (k == 0) return *p_zero;
    if (k == 1) return x;
    auto h = k >> 1, ibar = i;
    i += i;
    if (pool[i] == nullptr)
    {
        pool[i] = std::make_shared<Ciphertext>(x + x);
        counters[ADD]++;
    }
    auto y = muladd(*pool[i], h, i);
    if (!(k & 1)) return y;
    i += ibar;                            // correction term to account for odd numbers
    if (pool[i] == nullptr)
    {
        pool[i] = std::make_shared<Ciphertext>(y + x);
        counters[ADD]++;
    }
    return *pool[i];
}

template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::mulsaddfull(int a)
{
    if (pool.size() > a && pool[a] != nullptr) return *pool[a]; // the item is in the bool
    else // try to find combinations of values that add to the item
    {
        int half = a >> 1; // we don't need to iterate to the end since there can only be one value larger than a/2
        auto vidx = validIndices(a);
        if (!vidx.empty())  // if there is no value <= a/2 in the pool, there is no way of calculating by adding items
        {
            for (int d=2; d <= depth && d < eqsaddp2(a); d++) // the number of additions is d-1, so if d-1 is not less than eqsaddp2(a), it is better to use mulsaddp2(a)
            {
                std::pair<std::vector<int>,std::vector<int>> idx;
                idx.first = std::vector<int>(d-1, vidx.front()); // create an array of indices with d-1 items initialized to the smallest value in the pool
                idx.second = std::vector<int>(d-1, 0); // for indirect reference
                if (numpy::sum(idx.first) > half) break; // if the sum of smallest values is larger than a, than it is not possible to calculate it
                // check if there is any combitation that adds to a
                bool valid;
                do
                {
                    int i = a - numpy::sum(idx.first); // since the indices must add to a, i must be a minus the sum of the other indices
                    if (i > 0 && pool[i] != nullptr)
                    {
                        idx.first.push_back(i);
                        std::vector<Ciphertext> poolValues;
                        for (auto j : idx.first) poolValues.push_back(*pool[j]);
                        pool[a] = std::make_shared<Ciphertext>( sum_inplace(poolValues, idx.first) ); // sum ciphertexts
                        return *pool[a];
                    }
                    valid = increment(idx, vidx, half);
                } while (valid);
            }
        }
    }
    return mulsaddp2(a);
}

// selects which method to use based on number of additions.
template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::mulsaddp2(int k)
{
    if (eqsap2(k) < eqadd(k)) return mulsap2(k);
    int i = 1;
    return muladd(x, k, i);
}

// rounds k to the nearest power of 2 then subtracts it by the
// difference between it and k. Ex: 6 = 8 - 2
template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::mulsap2(int k)
{
    if (k == 0) return *p_zero;
    auto ck = 1 << math::clog2(k);
    auto lk = ck - k;
    int i = 1;
    auto xk = mulsap2_core(x, ck, 0, i);
    if (lk) { xk -= mulsap2_core(x, lk, 0, i); counters[SUB]++; }
    return xk;
}

template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::mulsap2_core(const Ciphertext & x, int k, int c, int & i)
{
    auto lk = 1 << math::flog2(k);
    auto idx = lk + c;
    if (pool[idx] != nullptr)
    {
        auto diff = k - lk; // for every non-power of two number, it computes the difference.
        if(diff)
        {
            counters[ADD]++;
            return *pool[idx] + mulsap2_core(x, diff, c, i);
        }
        return *pool[idx];
    }
    auto h = k >> 1, d = c + h, ibar = i; // c is the another correction term to keep track of the
    i += i; // original index wanted, d is it's next iteration.
    Ciphertext y;
    if (pool[i] != nullptr) y = *pool[i];
    else
    {
        y = x + x; counters[ADD]++;
        pool[i] = std::make_shared<Ciphertext>( Ciphertext(y) );
    }
    y = mulsap2_core(y, h, d, i);
    if (!(k & 1)) return y; // correction term to account for odd numbers
    i += ibar; // it also saves the result on pool.
    if (pool[i] != nullptr) y = *pool[i];
    else
    {
        y += x; counters[ADD]++;
        pool[i] = std::make_shared<Ciphertext>( Ciphertext(y) );
    }
    return y;
}

template <class Ciphertext>
void Stateful<Ciphertext>::resetPool()
{
    pool.clear();
    pool[0] = p_zero;
    pool[1] = std::make_shared<Ciphertext>(x);
}

template <class Ciphertext>
Ciphertext Stateful<Ciphertext>::sum_inplace(std::vector<Ciphertext> & values, std::vector<int> & indices)
{
    auto size = values.size();
    for ( auto n = 1 << math::msb(size); n > 1; )
    {
        n >>= 1;
        for ( int i = n; i < size; i++ )
        {
            values[i - n] += values[i];
            counters[ADD]++;
            indices[i - n] += indices[i];
            pool[indices[i - n]] = std::make_shared<Ciphertext>( Ciphertext(values[i - n]) );
        }
        size = n;
    }
    values.resize(1);
    return values[0];
}

template <class Ciphertext>
std::vector<int> Stateful<Ciphertext>::validIndices(int a)
{
    int half = a >> 1;
    std::vector<int> idx;
    for (int i = 0; i <= half; i++)
        if (pool.size() > i && pool[i] != nullptr) idx.push_back(i);
    return idx;
}

template <class Ciphertext>
void Stateful<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::Stateful<Ciphertext> & ct)
{
    return to_string( Ciphertext(ct) );
}

} // std
