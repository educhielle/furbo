#pragma once

#include <memory>
#include <vector>
#include "seal/seal.h"
#include "seal_bfv_keys.h"

namespace seal_wrapper
{

class SealBFVCiphertext;

class SealBFVPlaintext
{
    friend class SealBFVCiphertext;

    private:
        seal::Plaintext pt;
        std::shared_ptr<SealBFVKeys> keys;
        static std::shared_ptr<SealBFVKeys> default_keys;

    public:
        SealBFVPlaintext() {}
        SealBFVPlaintext(int);
        SealBFVPlaintext(uint64_t);
        SealBFVPlaintext(const std::vector<int> &);
        SealBFVPlaintext(const std::vector<uint64_t> &);
        SealBFVPlaintext(const SealBFVPlaintext &);
        SealBFVPlaintext(int, const SealBFVKeys &);
        SealBFVPlaintext(uint64_t, const SealBFVKeys &);
        SealBFVPlaintext(const std::vector<int> &, const SealBFVKeys &);
        SealBFVPlaintext(const std::vector<uint64_t> &, const SealBFVKeys &);

        explicit operator int();
        explicit operator uint64_t();
        explicit operator std::vector<int>();
        explicit operator std::vector<uint64_t>();

        std::vector<uint64_t> decode() const;
        std::vector<int> decode_int() const;
        int plaintextModulus() const;
        int polynomialDegree() const;

        static SealBFVKeys defaultKeys(const SealBFVKeys & keys = *default_keys);
};

} // seal_wrapper
