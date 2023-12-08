#pragma once

#include <iostream>
#include <utility>
#include "common.h"
#include "pool.h"

namespace smart
{

template <class Ciphertext>
class PartialState
{
    private:
        Ciphertext x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        mutable Pool<Ciphertext> pool;
        static std::vector<int> counters;
        static int default_modulus;
        static std::shared_ptr<Ciphertext> default_zero;

        PartialState & calc(const PartialState & a, Operation op);
        PartialState & calc(int a, Operation op);

        Ciphertext muladd(const Ciphertext & x, int k, int i);
        Ciphertext mulsaddp2(int k);
        Ciphertext mulsap2(int k);
        Ciphertext mulsap2_core(const Ciphertext & x, int k, int i);

        void resetPool();

    public:
        // constructors
        PartialState() {}
        PartialState(const PartialState & a)
            : PartialState(a.x, a.t, a.p_zero, a.pool) {}
        PartialState(const Ciphertext & ct)
            : PartialState(ct, default_modulus, default_zero, Pool<Ciphertext>(ct)) {}
        PartialState(const Ciphertext & ct, int mod)
            : PartialState(ct, mod, default_zero, Pool<Ciphertext>(ct)) {}
        PartialState(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : PartialState(ct, default_modulus, zero, Pool<Ciphertext>(ct)) {}
        PartialState(const Ciphertext & ct, const Pool<Ciphertext> & pool)
            : PartialState(ct, default_modulus, default_zero, pool) {}
        PartialState(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
            : PartialState(ct, mod, zero, Pool<Ciphertext>(ct)) {}
        PartialState(const Ciphertext & ct, int mod, const Pool<Ciphertext> & pool)
            : PartialState(ct, mod, default_zero, pool) {}
        PartialState(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & pool)
            : PartialState(ct, default_modulus, zero, pool) {}
        PartialState(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & p)
            : x(ct), t(mod), p_zero(zero), pool(p) {}

        // casting operators
        inline explicit operator Ciphertext() const { return x; }

        // compound assignment operators
        PartialState & operator +=(const PartialState & a) { calc(a, ADD); resetPool(); return *this; }
        PartialState & operator *=(const PartialState & a) { calc(a, MUL); resetPool(); return *this; }
        PartialState & operator -=(const PartialState & a) { calc(a, SUB); resetPool(); return *this; }

        PartialState & operator +=(int a) { calc(a, ADD); resetPool(); return *this; }
        PartialState & operator *=(int a) { calc(a, MUL); resetPool(); return *this; }
        PartialState & operator -=(int a) { calc(a, SUB); resetPool(); return *this; }

        // const operators
        PartialState operator +(const PartialState & a) const { PartialState r = *this; return r.calc(a, ADD); }
        PartialState operator *(const PartialState & a) const { PartialState r = *this; return r.calc(a, MUL); }
        PartialState operator -(const PartialState & a) const { PartialState r = *this; return r.calc(a, SUB); }

        PartialState operator +(int a) const { PartialState r = *this; return r.calc(a, ADD); }
        PartialState operator *(int a) const { PartialState r = *this; r.calc(a, MUL); pool = r.pool; r.pool.clear(); return r; }
        PartialState operator -(int a) const { PartialState r = *this; return r.calc(a, SUB); }

        // object functions
        Ciphertext getCiphertext() const { return x; }
        Pool<Ciphertext> getPool() const { return pool; }

        // class functions
        static int defaultModulus(int mod=default_modulus) { default_modulus = mod; return default_modulus; }
        static std::vector<int> getCounters() { return counters; }
        static Ciphertext getZero() { return *default_zero; }
        static void resetCounters() { counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0); };
        static void setZero(const Ciphertext & zero);

        // external functions
        friend std::ostream & operator <<(std::ostream & os, const PartialState & a) { return os << a.x; }
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::PartialState<Ciphertext> &);

} // std

#include "partialstate.hpp"
