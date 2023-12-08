#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "common.h"
#include "pool.h"

namespace smart
{

template <class Ciphertext>
class LogAdder
{
    private:
        Ciphertext x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        static std::vector<int> counters;
        static int default_modulus;
        static std::shared_ptr<Ciphertext> default_zero;

        LogAdder & calc(const LogAdder & a, Operation op);
        LogAdder & calc(int a, Operation op);

        Ciphertext muladd(const Ciphertext & x, int k);

    public:
        // constructors
        LogAdder() {}
        LogAdder(const LogAdder & a) : LogAdder(a.x, a.t, a.p_zero) {}
        LogAdder(const Ciphertext & ct) : LogAdder(ct, default_modulus, default_zero) {}
        LogAdder(const Ciphertext & ct, int mod) : LogAdder(ct, mod, default_zero) {}
        LogAdder(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : LogAdder(ct, default_modulus, zero) {}
        LogAdder(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
            : x(ct), t(mod), p_zero(zero) {}

        // casting operators
        inline explicit operator Ciphertext() const { return x; }

        // compound assignment operators
        LogAdder & operator +=(const LogAdder & a) { return calc(a, ADD); }
        LogAdder & operator *=(const LogAdder & a) { return calc(a, MUL); }
        LogAdder & operator -=(const LogAdder & a) { return calc(a, SUB); }

        LogAdder & operator +=(int a) { return calc(a, ADD); }
        LogAdder & operator *=(int a) { return calc(a, MUL); }
        LogAdder & operator -=(int a) { return calc(a, SUB); }

        // const operators
        LogAdder operator +(const LogAdder & a) const { LogAdder r = *this; return r.calc(a, ADD); }
        LogAdder operator *(const LogAdder & a) const { LogAdder r = *this; return r.calc(a, MUL); }
        LogAdder operator -(const LogAdder & a) const { LogAdder r = *this; return r.calc(a, SUB); }

        LogAdder operator +(int a) const { LogAdder r = *this; return r.calc(a, ADD); }
        LogAdder operator *(int a) const { LogAdder r = *this; return r.calc(a, MUL); }
        LogAdder operator -(int a) const { LogAdder r = *this; return r.calc(a, SUB); }

        // object functions
        Ciphertext getCiphertext() const { return x; }

        // class functions
        static int defaultModulus(int mod=default_modulus) { default_modulus = mod; return default_modulus; }
        static std::vector<int> getCounters() { return counters; }
        static Ciphertext getZero() { return *default_zero; }
        static void resetCounters() { counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0); };
        static void setZero(const Ciphertext & zero);

        // external functions
        friend std::ostream & operator <<(std::ostream & os, const LogAdder & a) { return os << a.x; }
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::LogAdder<Ciphertext> &);

} // std

#include "logadder.hpp"
