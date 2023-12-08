#pragma once

#include "math.h"
#include "numpy.h"

namespace smart
{

template <class Ciphertext>
std::vector<int> MemoryMul<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
int MemoryMul<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
std::shared_ptr<Ciphertext> MemoryMul<Ciphertext>::default_zero;

template <class Ciphertext>
MemoryMul<Ciphertext>::MemoryMul(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
{
    x = std::make_shared<Ciphertext>(ct);
    t = mod;
    p_zero = zero;
}

// compound assignment operators
template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator +=(const MemoryMul<Ciphertext> & a)
{
    counters[ADD]++;
    resetPool();
    x = std::make_shared<Ciphertext>(*x + *a.x);
    return *this;
}

template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator *=(const MemoryMul<Ciphertext> & a)
{
    counters[MUL]++;
    resetPool();
    x = std::make_shared<Ciphertext>(*x * *a.x);
    return *this;
}

template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator -=(const MemoryMul<Ciphertext> & a)
{
    counters[SUB]++;
    resetPool();
    x = std::make_shared<Ciphertext>(*x - *a.x);
    return *this;
}

template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator +=(int a)
{
    counters[N_OPERATIONS + ADD]++;
    resetPool();
    math::reduce(a, t);
    x = std::make_shared<Ciphertext>(*x + a);
    return *this;
}

template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator *=(int a)
{
    math::reduce(a, t);
    if ( pool[a] == nullptr )
    {
        counters[N_OPERATIONS + MUL]++;
        pool[a] = std::make_shared<Ciphertext>(*x * a);
    }
    else x = pool[a];
    resetPool();
    return *this;
}

template <class Ciphertext>
MemoryMul<Ciphertext> & MemoryMul<Ciphertext>::operator -=(int a)
{
    counters[N_OPERATIONS + SUB]++;
    resetPool();
    math::reduce(a, t);
    x = std::make_shared<Ciphertext>(*x - a);
    return *this;
}

// const operators
template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator +(const MemoryMul<Ciphertext> & a) const
{
    counters[ADD]++;
    MemoryMul<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x + *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator *(const MemoryMul<Ciphertext> & a) const
{
    counters[MUL]++;
    MemoryMul<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x * *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator -(const MemoryMul<Ciphertext> & a) const
{
    counters[SUB]++;
    MemoryMul<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x - *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator +(int a) const
{
    counters[N_OPERATIONS + ADD]++;
    math::reduce(a, t);
    MemoryMul<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x + a);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator *(int a) const
{
    MemoryMul<Ciphertext> r;
    math::reduce(a, t);
    if ( pool[a] == nullptr )
    {
        counters[N_OPERATIONS + MUL]++;
        pool[a] = std::make_shared<Ciphertext>(*x * a);
    }
    r.x = pool[a];
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryMul<Ciphertext> MemoryMul<Ciphertext>::operator -(int a) const
{
    counters[N_OPERATIONS + SUB]++;
    math::reduce(a, t);
    MemoryMul<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x - a);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
inline void MemoryMul<Ciphertext>::resetPool()
{
    pool = Pool<Ciphertext>();
}

template <class Ciphertext>
void MemoryMul<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

template <class Ciphertext>
std::ostream & operator <<(std::ostream & os, const MemoryMul<Ciphertext> & a)
{
    if ( a.x != nullptr ) return os << *a.x;
    if ( a.p_zero != nullptr ) return os << *a.p_zero;
    if ( MemoryMul<Ciphertext>::default_zero != nullptr ) return os << *MemoryMul<Ciphertext>::default_zero;
    return os << Ciphertext(0);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::MemoryMul<Ciphertext> & ct)
{
    try
    {
        return to_string( Ciphertext(ct) );
    }
    catch (...) {}
    return Ciphertext(0);
}

} // std
