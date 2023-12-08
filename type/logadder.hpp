#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "math.h"

namespace smart
{

template <class Ciphertext>
std::vector<int> LogAdder<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
int LogAdder<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
std::shared_ptr<Ciphertext> LogAdder<Ciphertext>::default_zero;


template <class Ciphertext>
LogAdder<Ciphertext> & LogAdder<Ciphertext>::calc(const LogAdder<Ciphertext> & a, Operation op)
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
LogAdder<Ciphertext> & LogAdder<Ciphertext>::calc(int a, Operation op)
{
    math::reduce(a, t);
    switch (op)
    {
        case ADD : x += a;            break;
        case MUL : x  = muladd(x, a); break;
        case SUB : x -= a;            break;
        default : throw "Operator not supported";
    }
    if (op != MUL) { counters[N_OPERATIONS + op]++; }
    return *this;
}

// computes result recursively by calculating lesser
// coefficients then adding them.
template <class Ciphertext>
Ciphertext LogAdder<Ciphertext>::muladd(const Ciphertext & x, int k)
{
    if (k == 0) return *p_zero;
    if (k == 1) return x;
    auto h = k >> 1;
    auto y = x + x; counters[ADD]++;
    y = muladd(y, h);
    if (k & 1) { y += x; counters[ADD]++; } // correction term to account for odd numbers.
    return y;
}

template <class Ciphertext>
void LogAdder<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::LogAdder<Ciphertext> & ct)
{
    return to_string( Ciphertext(ct) );
}

} // std
