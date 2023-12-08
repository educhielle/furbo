#pragma once

#include "math.h"

namespace smart
{

template <class Ciphertext>
int PartialState<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
std::vector<int> PartialState<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
std::shared_ptr<Ciphertext> PartialState<Ciphertext>::default_zero;


template <class Ciphertext>
PartialState<Ciphertext> & PartialState<Ciphertext>::calc(const PartialState<Ciphertext> & a, Operation op)
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
PartialState<Ciphertext> & PartialState<Ciphertext>::calc(int a, Operation op)
{
    math::reduce(a, t);
    switch (op)
    {
        case ADD : x += a;            break;
        case MUL : x  = mulsaddp2(a); break;
        case SUB : x -= a;            break;
        default : throw "Operator not supported";
    }
    if (op != MUL) { counters[N_OPERATIONS + op]++; }
    return *this;
}

// computes result recursively by calculating lesser
// coefficients then adding them.
template <class Ciphertext>
Ciphertext PartialState<Ciphertext>::muladd(const Ciphertext & x, int k, int i)
{
    if (k == 0) return *p_zero;
    if (k == 1) return x;
    auto h = k >> 1;

    if (i >= pool.size())
    {                                  // if pool is smaller than i, it does not have the coefficient.
        pool.push_back(x + x);
        counters[ADD]++;
    }
    auto y = muladd(*pool.back(), h, ++i);
    if (k & 1) { y += x; counters[ADD]++; } // correction term to account for odd numbers.
    return y;
}

// selects which method to use based on number of additions.
template <class Ciphertext>
Ciphertext PartialState<Ciphertext>::mulsaddp2(int k)
{
    if (eqsap2(k) < eqadd(k)) return mulsap2(k);
    return muladd(x, k, 1);
}

// rounds k to the nearest power of 2 then subtracts it by the
// difference between it and k. Ex: 6 = 8 - 2
template <class Ciphertext>
Ciphertext PartialState<Ciphertext>::mulsap2(int k)
{
    if (k == 0) return *p_zero;
    auto ck = 1 << math::clog2(k);
    auto lk = ck - k;
    auto xk = mulsap2_core(x, ck, 0);
    if (lk)
    {
        xk -= mulsap2_core(x, lk, 0); // this subtraction may create transparent ciphertext
        counters[SUB]++;
    }
    return xk;
}

template <class Ciphertext>
Ciphertext PartialState<Ciphertext>::mulsap2_core(const Ciphertext & x, int k, int i)
{
    auto flogk = math::flog2(k);
    auto idx = flogk + i;                               // idx dictates what power of two it needs, the position on the
    if ( idx < pool.size() )                            // pool indicates the exponent. Ex: 8 = 2^3, so pool[3]=8a
    {                                                   // if pool is smaller than idx, it does not have the coefficient.
        auto y = pool[idx];
        auto diff = k - (1 << flogk);
        if (diff)                                       // for every non-power of two number, it computes the difference.
        {
            counters[ADD]++;
            return *y + mulsap2_core(x, diff, i);
        }
        return *y;
    }
    auto h = k >> 1;

    if ( ++i >= pool.size() )
    {
        pool.push_back(x + x);
        counters[ADD]++;
    }
    auto y = mulsap2_core(*pool[i], h, i);
    if (k & 1) { y += x; counters[ADD]++; }             // correction term to account for odd numbers.
    return y;
}

template <class Ciphertext>
void PartialState<Ciphertext>::resetPool()
{
    pool = Pool<Ciphertext>(x);
}

template <class Ciphertext>
void PartialState<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::PartialState<Ciphertext> & ct)
{
    return to_string( Ciphertext(ct) );
}

} // std
