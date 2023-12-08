#pragma once

#include "math.h"

namespace smart
{

template <class Ciphertext>
int Stateless<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
std::vector<int> Stateless<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
std::shared_ptr<Ciphertext> Stateless<Ciphertext>::default_zero;


template <class Ciphertext>
inline Stateless<Ciphertext> & Stateless<Ciphertext>::calc(const Stateless<Ciphertext> & a, Operation op)
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
inline Stateless<Ciphertext> & Stateless<Ciphertext>::calc(int a, Operation op)
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
Ciphertext Stateless<Ciphertext>::muladd(const Ciphertext & x, int k)
{
    if (k == 0) return *p_zero;
    if (k == 1) return x;
    auto h = k >> 1;
    auto y = x + x; counters[ADD]++;
    y = muladd(y, h);
    if (k & 1) { y += x; counters[ADD]++; } // correction term to account for odd numbers.
    return y;
}

// selects which method to use based on number of additions.
template <class Ciphertext>
Ciphertext Stateless<Ciphertext>::mulsaddp2(int k)
{
    if (eqsap2(k) < eqadd(k)) return mulsap2(k);
    return muladd(x, k);
}

// rounds k to the nearest power of 2 then subtracts it by the
// difference between it and k. Ex: 6 = 8 - 2
template <class Ciphertext>
Ciphertext Stateless<Ciphertext>::mulsap2(int k)
{
    if (k == 0) return *p_zero;
    auto ck = 1 << math::clog2(k);
    auto lk = ck - k;
    Pool pool(x); // Creates pool and trashes it, only to implement algorithm.
    auto xk = mulsap2_core(x, ck, pool, 0);
    if (lk) { xk -= mulsap2_core(x, lk, pool, 0); counters[SUB]++; }
    return xk;
}

template <class Ciphertext>
Ciphertext Stateless<Ciphertext>::mulsap2_core(const Ciphertext & x, int k, Pool<Ciphertext> & pool, int i)
{
    auto flogk = math::flog2(k);
    auto idx = flogk + i;                               // idx dictates what power of two it needs, the position on the
    if ( idx < pool.size() )
    {
        auto diff = k - (1 << flogk);
        if (diff)
        {
            counters[ADD]++;
            return *pool[idx] + mulsap2_core(x, diff, pool, i);
        }
        return *pool[idx];
    }
    auto h = k >> 1;
    pool.push_back(x+x);
    counters[ADD]++;
    auto y = mulsap2_core(*pool.back(), h, pool, ++i);
    if (k & 1) { y += x; counters[ADD]++; }             // correction term to account for odd numbers.
    return y;
}

template <class Ciphertext>
void Stateless<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::Stateless<Ciphertext> & ct)
{
    return to_string( Ciphertext(ct) );
}

} // std
