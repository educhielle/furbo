#pragma once

#include <iostream>
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
class MemoryType
{
    private:
        std::shared_ptr<const Ciphertext> x;
        int t; // plaintext modulus
        std::shared_ptr<Ciphertext> p_zero;
        mutable Pool<MemoryType<Ciphertext>> pool;
        static std::vector<int> counters;
        static int default_modulus;
        static std::shared_ptr<Ciphertext> default_zero;

        void resetPool();

    public:
        // constructors
        MemoryType() {}
        MemoryType(const MemoryType & a)
            : x(a.x), t(a.t), p_zero(a.p_zero), pool(a.pool) {}
        MemoryType(const Ciphertext & ct)
            : MemoryType(ct, default_modulus, default_zero) {}
        MemoryType(const Ciphertext & ct, int mod)
            : MemoryType(ct, mod, default_zero) {}
        MemoryType(const Ciphertext & ct, const std::shared_ptr<Ciphertext> & zero)
            : MemoryType(ct, default_modulus, zero) {}
        MemoryType(const std::shared_ptr<Ciphertext> & ct, int mod, const std::shared_ptr<Ciphertext> & zero)
            : x(ct), t(mod), p_zero(zero) {}
        MemoryType(const Ciphertext & ct, int mod, const std::shared_ptr<Ciphertext> & zero); // <-- implemented

        // casting operators
        inline explicit operator Ciphertext() const { return *x; }

        // compound assignment operators
        MemoryType & operator +=(const MemoryType & a);
        MemoryType & operator *=(const MemoryType & a);
        MemoryType & operator -=(const MemoryType & a);

        MemoryType & operator +=(int a);
        MemoryType & operator *=(int a);
        MemoryType & operator -=(int a);

        // const operators
        MemoryType operator +(const MemoryType & a) const;
        MemoryType operator *(const MemoryType & a) const;
        MemoryType operator -(const MemoryType & a) const;

        MemoryType operator +(int a) const;
        MemoryType operator *(int a) const;
        MemoryType operator -(int a) const;

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
        friend std::ostream & operator <<(std::ostream & os, const MemoryType<T> & a);
};

} // smart

namespace std
{

template <class Ciphertext>
string to_string(const smart::MemoryType<Ciphertext> &);

} // std

#include "memorytype.hpp"
