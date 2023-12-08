#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include "common.h"
#include "pool.h"

namespace smart
{

template <class Ciphertext>
class Stateful
{
    private:
        Ciphertext x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        mutable Pool<Ciphertext> pool;
        static std::vector<int> counters;
        static int default_modulus;
        static int depth;
        static std::shared_ptr<Ciphertext> default_zero;

        Stateful & calc(const Stateful & a, Operation op);
        Stateful & calc(int a, Operation op);

        Ciphertext muladd(const Ciphertext & x, int k, int & i);
        Ciphertext mulsaddp2(int k);
        Ciphertext mulsap2(int k);
        Ciphertext mulsap2_core(const Ciphertext & x, int k, int c, int & i);

        bool increment(std::pair<std::vector<int>,std::vector<int>> & idx, const std::vector<int> & vidx, int half);
        Ciphertext mulsaddfull(int k);
        Ciphertext sum_inplace(std::vector<Ciphertext> & values, std::vector<int> & indices);
        std::vector<int> validIndices(int a);

        void resetPool();

    public:
        // constructors
        Stateful() {}
        Stateful(const Stateful & a)
            : Stateful(a.x, a.t, a.p_zero, a.pool) {}
        Stateful(const Ciphertext & ct)
            : Stateful(ct, default_modulus, default_zero) {}
        Stateful(const Ciphertext & ct, int mod)
            : Stateful(ct, mod, default_zero) {}
        Stateful(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : Stateful(ct, default_modulus, zero) {}
        Stateful(const Ciphertext & ct, const Pool<Ciphertext> & pool)
            : Stateful(ct, default_modulus, default_zero, pool) {}
        Stateful(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero); // <-- implemented
        Stateful(const Ciphertext & ct, int mod, const Pool<Ciphertext> & pool)
            : Stateful(ct, mod, default_zero, pool) {}
        Stateful(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & pool)
            : Stateful(ct, default_modulus, zero, pool) {}
        Stateful(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & p)
            : x(ct), t(mod), p_zero(zero), pool(p) {}

        // casting operators
        inline explicit operator Ciphertext() const { return x; }

        // compound assignment operators
        Stateful & operator +=(const Stateful & a) { calc(a, ADD); resetPool(); return *this; }
        Stateful & operator *=(const Stateful & a) { calc(a, MUL); resetPool(); return *this; }
        Stateful & operator -=(const Stateful & a) { calc(a, SUB); resetPool(); return *this; }

        Stateful & operator +=(int a) { calc(a, ADD); resetPool(); return *this; }
        Stateful & operator *=(int a) { calc(a, MUL); resetPool(); return *this; }
        Stateful & operator -=(int a) { calc(a, SUB); resetPool(); return *this; }

        // const operators
        Stateful operator +(const Stateful & a) const { Stateful r = *this; return r.calc(a, ADD); }
        Stateful operator *(const Stateful & a) const { Stateful r = *this; return r.calc(a, MUL); }
        Stateful operator -(const Stateful & a) const { Stateful r = *this; return r.calc(a, SUB); }

        Stateful operator +(int a) const { Stateful r = *this; return r.calc(a, ADD); }
        Stateful operator *(int a) const { Stateful r = *this; r.calc(a, MUL); pool = r.pool; r.resetPool(); return r; }
        Stateful operator -(int a) const { Stateful r = *this; return r.calc(a, SUB); }

        // object functions
        Ciphertext getCiphertext() const { return x; }
        Pool<Ciphertext> getPool() const { return pool; }

        // class functions
        static int defaultModulus(int mod=default_modulus) { default_modulus = mod; return default_modulus; }
        static std::vector<int> getCounters() { return counters; }
        static Ciphertext getZero() { return *default_zero; }
        static void resetCounters() { counters = std::vector<int>(N_OPERATIONS * N_OP_TYPES, 0); };
        static int searchDepth(int d=depth) { depth = d; return depth; }
        static void setZero(const Ciphertext & zero);

        // external functions
        friend std::ostream & operator <<(std::ostream & os, const Stateful & a) { return os << a.x; }
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::Stateful<Ciphertext> &);

} // std

#include "stateful.hpp"
