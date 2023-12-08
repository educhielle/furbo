#pragma once

#include <iostream>
#include <utility>
#include "common.h"
#include "pool.h"

namespace smart
{

template <class Ciphertext>
class Stateless
{
    private:
        Ciphertext x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        static std::vector<int> counters;
        static int default_modulus;
        static std::shared_ptr<Ciphertext> default_zero;

        Stateless & calc(const Stateless & a, Operation op);
        Stateless & calc(int a, Operation op);

        Ciphertext muladd(const Ciphertext & x, int k);
        Ciphertext mulsaddp2(int k);
        Ciphertext mulsap2(int k);
        Ciphertext mulsap2_core(const Ciphertext & x, int k, Pool<Ciphertext> & pool, int i);

    public:
        // constructors
        Stateless() {}
        Stateless(const Stateless & a) : Stateless(a.x, a.t, a.p_zero) {}
        Stateless(const Ciphertext & ct) : Stateless(ct, default_modulus, default_zero) {}
        Stateless(const Ciphertext & ct, int mod) : Stateless(ct, mod, default_zero) {}
        Stateless(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : Stateless(ct, default_modulus, zero) {}
        Stateless(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
            : x(ct), t(mod), p_zero(zero) {}

        // casting operators
        inline explicit operator Ciphertext() const { return x; }

        // compound assignment operators
        Stateless & operator +=(const Stateless & a) { return calc(a, ADD); }
        Stateless & operator *=(const Stateless & a) { return calc(a, MUL); }
        Stateless & operator -=(const Stateless & a) { return calc(a, SUB); }

        Stateless & operator +=(int a) { return calc(a, ADD); }
        Stateless & operator *=(int a) { return calc(a, MUL); }
        Stateless & operator -=(int a) { return calc(a, SUB); }

        // const operators
        Stateless operator +(const Stateless & a) const { Stateless r = *this; return r.calc(a, ADD); }
        Stateless operator *(const Stateless & a) const { Stateless r = *this; return r.calc(a, MUL); }
        Stateless operator -(const Stateless & a) const { Stateless r = *this; return r.calc(a, SUB); }

        Stateless operator +(int a) const { Stateless r = *this; return r.calc(a, ADD); }
        Stateless operator *(int a) const { Stateless r = *this; return r.calc(a, MUL); }
        Stateless operator -(int a) const { Stateless r = *this; return r.calc(a, SUB); }

        // object functions
        Ciphertext getCiphertext() const { return x; }

        // class functions
        static int defaultModulus(int mod=default_modulus) { default_modulus = mod; return default_modulus; }
        static std::vector<int> getCounters() { return counters; }
        static Ciphertext getZero() { return *default_zero; }
        static void resetCounters() { counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0); };
        static void setZero(const Ciphertext & zero);

        // external functions
        friend std::ostream & operator <<(std::ostream & os, const Stateless & a) { return os << a.x; }
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::Stateless<Ciphertext> &);

} // std

#include "stateless.hpp"
