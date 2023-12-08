#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include "common.h"

#if (SHARED_POOL == 1)
    #pragma message "Using shared pool."
    #include "pool_shared.h"
#else
    #pragma message "Using standard pool."
    #include "pool.h"
#endif

namespace smart
{

template <class Ciphertext>
class MemoryMul
{
    private:
        std::shared_ptr<Ciphertext> x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        mutable Pool<Ciphertext> pool;
        static std::vector<int> counters;
        static int default_modulus;
        static std::shared_ptr<Ciphertext> default_zero;

        void resetPool();

    public:
        // constructors
        MemoryMul() {}
        MemoryMul(const MemoryMul & a)
            : x(a.x), t(a.t), p_zero(a.p_zero), pool(a.pool) {}
        MemoryMul(const Ciphertext & ct)
            : MemoryMul(ct, default_modulus, default_zero) {}
        MemoryMul(const Ciphertext & ct, int mod)
            : MemoryMul(ct, mod, default_zero) {}
        MemoryMul(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : MemoryMul(ct, default_modulus, zero) {}
        MemoryMul(const Ciphertext & ct, const Pool<Ciphertext> & pool)
            : MemoryMul(ct, default_modulus, default_zero, pool) {}
        MemoryMul(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero); // <-- implemented
        MemoryMul(const Ciphertext & ct, int mod, const Pool<Ciphertext> & pool)
            : MemoryMul(ct, mod, default_zero, pool) {}
        MemoryMul(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & pool)
            : MemoryMul(ct, default_modulus, zero, pool) {}
        MemoryMul(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero, const Pool<Ciphertext> & p)
            : x(std::make_shared<Ciphertext>(ct)), t(mod), p_zero(zero), pool(p) {}

        // casting operators
        inline explicit operator Ciphertext() const { return *x; }

        // compound assignment operators
        MemoryMul & operator +=(const MemoryMul & a);
        MemoryMul & operator *=(const MemoryMul & a);
        MemoryMul & operator -=(const MemoryMul & a);

        MemoryMul & operator +=(int a);
        MemoryMul & operator *=(int a);
        MemoryMul & operator -=(int a);

        // const operators
        MemoryMul operator +(const MemoryMul & a) const;
        MemoryMul operator *(const MemoryMul & a) const;
        MemoryMul operator -(const MemoryMul & a) const;

        MemoryMul operator +(int a) const;
        MemoryMul operator *(int a) const;
        MemoryMul operator -(int a) const;

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
        template <class T>
        friend std::ostream & operator <<(std::ostream & os, const MemoryMul<T> & a);
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::MemoryMul<Ciphertext> &);

} // std

#include "memorymul.hpp"
