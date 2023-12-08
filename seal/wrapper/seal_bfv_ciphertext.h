#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "seal/seal.h"
#include "seal_bfv_common.h"
#include "seal_bfv_keys.h"
#include "seal_bfv_plaintext.h"

extern int nativeCopyCounter;

namespace seal_wrapper
{

class SealBFVCiphertext
{
    private:
        seal::Ciphertext ct;
        int id;
        std::shared_ptr<SealBFVKeys> keys;

        static std::vector<int> counters;
        static std::shared_ptr<SealBFVKeys> default_keys;
        static int id_counter;
        static PrintingMode printing_mode;

        void newId();
        seal::Ciphertext pow(const seal::Ciphertext &, int);

    public:
        SealBFVCiphertext();
        SealBFVCiphertext(int);
        SealBFVCiphertext(uint64_t);
        SealBFVCiphertext(const std::vector<int> &);
        SealBFVCiphertext(const std::vector<uint64_t> &);
        SealBFVCiphertext(const SealBFVPlaintext &);
        SealBFVCiphertext(const SealBFVCiphertext &);
        SealBFVCiphertext(int, const SealBFVKeys &);
        SealBFVCiphertext(int, const std::shared_ptr<SealBFVKeys> &);
        SealBFVCiphertext(uint64_t, const SealBFVKeys &);
        SealBFVCiphertext(uint64_t, const std::shared_ptr<SealBFVKeys> &);
        SealBFVCiphertext(const std::vector<int> &, const SealBFVKeys &);
        SealBFVCiphertext(const std::vector<int> &, const std::shared_ptr<SealBFVKeys> &);
        SealBFVCiphertext(const std::vector<uint64_t> &, const SealBFVKeys &);
        SealBFVCiphertext(const std::vector<uint64_t> &, const std::shared_ptr<SealBFVKeys> &);

        explicit operator int() const;
        explicit operator uint64_t() const;
        explicit operator std::vector<int>() const;
        explicit operator std::vector<uint64_t>() const;
        explicit operator SealBFVPlaintext() const;
        explicit operator std::string() const;

        // compound assignment operators
        SealBFVCiphertext & operator~(); // negate_inplace
        SealBFVCiphertext & operator++();
        SealBFVCiphertext & operator--();
        SealBFVCiphertext operator++(int);
        SealBFVCiphertext operator--(int);

        SealBFVCiphertext & operator +=(const SealBFVCiphertext &);
        SealBFVCiphertext & operator *=(const SealBFVCiphertext &);
        SealBFVCiphertext & operator -=(const SealBFVCiphertext &);

        SealBFVCiphertext & operator +=(const SealBFVPlaintext &);
        SealBFVCiphertext & operator *=(const SealBFVPlaintext &);
        SealBFVCiphertext & operator -=(const SealBFVPlaintext &);

        SealBFVCiphertext & operator +=(int);
        SealBFVCiphertext & operator *=(int);
        SealBFVCiphertext & operator -=(int);
        SealBFVCiphertext & operator ^=(int); // exponentation
        SealBFVCiphertext & operator <<=(int); // rotate rows left
        SealBFVCiphertext & operator >>=(int); // rotate rows right

        SealBFVCiphertext & operator +=(uint64_t);
        SealBFVCiphertext & operator *=(uint64_t);
        SealBFVCiphertext & operator -=(uint64_t);

        // const operators
        SealBFVCiphertext operator +() const;
        SealBFVCiphertext operator -() const; // negate
        SealBFVCiphertext operator !() const; // rotate columns

        SealBFVCiphertext operator +(const SealBFVCiphertext &) const;
        SealBFVCiphertext operator *(const SealBFVCiphertext &) const;
        SealBFVCiphertext operator -(const SealBFVCiphertext &) const;

        SealBFVCiphertext operator +(const SealBFVPlaintext &) const;
        SealBFVCiphertext operator *(const SealBFVPlaintext &) const;
        SealBFVCiphertext operator -(const SealBFVPlaintext &) const;

        SealBFVCiphertext operator +(int) const;
        SealBFVCiphertext operator *(int) const;
        SealBFVCiphertext operator -(int) const;
        SealBFVCiphertext operator ^(int) const; // exponentation
        SealBFVCiphertext operator <<(int) const; // rotate rows left
        SealBFVCiphertext operator >>(int) const; // rotate rows right

        SealBFVCiphertext operator +(uint64_t) const;
        SealBFVCiphertext operator *(uint64_t) const;
        SealBFVCiphertext operator -(uint64_t) const;

        friend SealBFVCiphertext operator+(const SealBFVPlaintext &, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator*(const SealBFVPlaintext &, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator-(const SealBFVPlaintext &, const SealBFVCiphertext &);

        friend SealBFVCiphertext operator+(int, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator*(int, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator-(int, const SealBFVCiphertext &);

        friend SealBFVCiphertext operator+(uint64_t, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator*(uint64_t, const SealBFVCiphertext &);
        friend SealBFVCiphertext operator-(uint64_t, const SealBFVCiphertext &);

        std::vector<uint64_t> decode() const;
        std::vector<int> decode_int() const;
        SealBFVPlaintext decrypt() const;
        std::shared_ptr<SealBFVKeys> getKeys() const;
        int plaintextModulus() const;
        int polynomialDegree() const;

        static SealBFVCiphertext add_many(const std::vector<SealBFVCiphertext> &);
        static SealBFVKeys defaultKeys(const SealBFVKeys & keys = *default_keys);
        static std::vector<int> getCounters();
        static SealBFVCiphertext mul_many(const std::vector<SealBFVCiphertext> &);
        static PrintingMode printingMode(const PrintingMode & = printing_mode);

        friend std::ostream & operator <<(std::ostream & os, const SealBFVCiphertext &);
};

} // seal_wrapper

namespace std
{

string to_string(const seal_wrapper::SealBFVCiphertext &);

} // std
