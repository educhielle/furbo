// this class simulates homomorphic encryption computation
#pragma once

#include <iostream>
#include "common.h"

namespace smart
{

class Ciphertext
{
    private:
        Ciphertext & calc(const Ciphertext & a, Operation op);
        Ciphertext & calc(int a, Operation op);

        int x;
        int t; // plaintext modulus
        inline static int default_modulus = 0;

    public:
        // constructors
        Ciphertext() : Ciphertext(0) {}
        Ciphertext(int ct) : Ciphertext(ct, default_modulus) {}
        Ciphertext(int ct, int t): x(ct), t(t) {}

        operator std::string() const { return std::to_string(x); }

        // compound assignment operators
        Ciphertext & operator +=(const Ciphertext & a) { return calc(a, ADD); }
        Ciphertext & operator -=(const Ciphertext & a) { return calc(a, SUB); }
        Ciphertext & operator *=(const Ciphertext & a) { return calc(a, MUL); }

        Ciphertext & operator +=(int a) { return calc(a, ADD); }
        Ciphertext & operator -=(int a) { return calc(a, SUB); }
        Ciphertext & operator *=(int a) { return calc(a, MUL); }

        // const operators
        Ciphertext operator +(const Ciphertext & a) const { Ciphertext r = *this; return r.calc(a, ADD); }
        Ciphertext operator -(const Ciphertext & a) const { Ciphertext r = *this; return r.calc(a, SUB); }
        Ciphertext operator *(const Ciphertext & a) const { Ciphertext r = *this; return r.calc(a, MUL); }

        Ciphertext operator +(int a) const { Ciphertext r = *this; return r.calc(a, ADD); }
        Ciphertext operator -(int a) const { Ciphertext r = *this; return r.calc(a, SUB); }
        Ciphertext operator *(int a) const { Ciphertext r = *this; return r.calc(a, MUL); }

        // functions
        static int defaultModulus(int mod=default_modulus) { default_modulus = mod; return default_modulus; }

        // external functions
        friend std::ostream & operator <<(std::ostream & os, const Ciphertext & a) { return os << a.x; }
};

} // smart
