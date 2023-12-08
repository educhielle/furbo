#pragma once

#include <iostream> // FIXME remove
#include "math.h"
#include "numpy.h"

namespace smart
{

template <class Ciphertext>
std::vector<int> MemoryType<Ciphertext>::counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0);

template <class Ciphertext>
int MemoryType<Ciphertext>::default_modulus = 0;

template <class Ciphertext>
std::shared_ptr<Ciphertext> MemoryType<Ciphertext>::default_zero;

template <class Ciphertext>
MemoryType<Ciphertext>::MemoryType(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
{
    x = std::make_shared<Ciphertext>(ct);
    t = mod;
    p_zero = zero;
}

// compound assignment operators
template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator +=(const MemoryType<Ciphertext> & a)
{
    if (a.x == p_zero) return *this;
    if (x == p_zero)
    {
        *this = a;
        return *this;
    }
    counters[ADD]++;
    resetPool();
    x = std::make_shared<Ciphertext>(*x + *a.x);
    return *this;
}

template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator *=(const MemoryType<Ciphertext> & a)
{
    if (x == p_zero) return *this;

    if (a.x == p_zero) x = p_zero;
    else
    {
        counters[MUL]++;
        x = std::make_shared<Ciphertext>(*x * *a.x);
    }
    resetPool();
    return *this;
}

template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator -=(const MemoryType<Ciphertext> & a)
{
    if (a.x == p_zero) return *this;

    counters[SUB]++;
    resetPool();
    x = std::make_shared<Ciphertext>(*x - *a.x);
    return *this;
}

template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator +=(int a)
{
    if (!a) return *this;

    counters[N_OPERATIONS + ADD]++;
    resetPool();
    math::reduce(a, t);
    x = std::make_shared<Ciphertext>(*x + a);
    return *this;
}

template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator *=(int a)
{
    if (x == p_zero) return *this;

    math::reduce(a, t);
    if (a == 1) return *this;

    if (!a) x = p_zero;
    else if ( pool[a] == nullptr )
    {
        counters[N_OPERATIONS + MUL]++;
        pool[a] = std::make_shared<MemoryType<Ciphertext>>( MemoryType<Ciphertext>(*x * a, t, p_zero) );
    }
    else x = pool[a]->x;
    resetPool();
    return *this;
}

template <class Ciphertext>
MemoryType<Ciphertext> & MemoryType<Ciphertext>::operator -=(int a)
{
    if (!a) return *this;

    counters[N_OPERATIONS + SUB]++;
    resetPool();
    math::reduce(a, t);
    x = std::make_shared<Ciphertext>(*x - a);
    return *this;
}

// const operators
template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator +(const MemoryType<Ciphertext> & a) const
{
    if (x == p_zero) return MemoryType<Ciphertext>(a);
    if (a.x == p_zero) return MemoryType<Ciphertext>(*this);

    counters[ADD]++;
    MemoryType<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x + *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator *(const MemoryType<Ciphertext> & a) const
{
    if (x == p_zero) return MemoryType<Ciphertext>(*this);
    if (a.x == p_zero) return MemoryType<Ciphertext>(a);

    counters[MUL]++;
    MemoryType<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x * *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator -(const MemoryType<Ciphertext> & a) const
{
    if (a.x == p_zero) return MemoryType<Ciphertext>(*this);

    counters[SUB]++;
    MemoryType<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x - *a.x);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator +(int a) const
{
    if (!a) return MemoryType<Ciphertext>(*this);

    counters[N_OPERATIONS + ADD]++;
    math::reduce(a, t);
    MemoryType<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x + a);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator *(int a) const
{
    if (!a) return MemoryType<Ciphertext>(p_zero, t, p_zero);
    if (a == 1) return MemoryType<Ciphertext>(*this);

    MemoryType<Ciphertext> r;
    math::reduce(a, t);
    if ( pool[a] == nullptr )
    {
        counters[N_OPERATIONS + MUL]++;
        pool[a] = std::make_shared<MemoryType<Ciphertext>>( MemoryType<Ciphertext>(*x * a, t, p_zero) );
    }
    r.x = pool[a]->x;
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
MemoryType<Ciphertext> MemoryType<Ciphertext>::operator -(int a) const
{
    if (!a) return MemoryType<Ciphertext>(*this);

    counters[N_OPERATIONS + SUB]++;
    math::reduce(a, t);
    MemoryType<Ciphertext> r;
    r.x = std::make_shared<Ciphertext>(*x - a);
    r.t = t;
    r.p_zero = p_zero;
    return r;
}

template <class Ciphertext>
inline void MemoryType<Ciphertext>::resetPool()
{
    pool = Pool<MemoryType<Ciphertext>>();
}

template <class Ciphertext>
void MemoryType<Ciphertext>::setZero(const Ciphertext & zero)
{
    default_zero = std::make_shared<Ciphertext>(zero);
}

template <class Ciphertext>
std::ostream & operator <<(std::ostream & os, const MemoryType<Ciphertext> & a)
{
    if ( a.x != nullptr ) return os << *a.x;
    if ( a.p_zero != nullptr ) return os << *a.p_zero;
    if ( MemoryType<Ciphertext>::default_zero != nullptr ) return os << *MemoryType<Ciphertext>::default_zero;
    return os << Ciphertext(0);
}

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::MemoryType<Ciphertext> & ct)
{
    try
    {
        return to_string( Ciphertext(ct) );
    }
    catch (...) {}
    return Ciphertext(0);
}

} // std
