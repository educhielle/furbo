#pragma once

#include <memory>
#include <string>
#include <vector>
#include "seal/seal.h"

namespace seal_wrapper
{

class SealBFVKeys
{
    private:
        int n; // polynomial modulus
        int t; // plaintext modulus
        seal::EncryptionParameters params;
        std::shared_ptr<seal::SEALContext> context;
        seal::SecretKey sk;
        seal::PublicKey pk;
        seal::RelinKeys rk;
        seal::GaloisKeys gk;
        seal::Encryptor * enc;
        seal::Decryptor * dec;
        seal::Evaluator * eval;
        seal::BatchEncoder * encoder;
        seal::Ciphertext zero;

        void createContext();

    public:
        SealBFVKeys() {}
        SealBFVKeys(int n, int t);
        SealBFVKeys(int n, int t, const seal::SecretKey &,
            const seal::PublicKey &, const seal::RelinKeys &,
            const seal::GaloisKeys &);
        SealBFVKeys(const SealBFVKeys &);
        ~SealBFVKeys();

        SealBFVKeys & operator =(const SealBFVKeys &);
        seal::Ciphertext add(const seal::Ciphertext &, const seal::Ciphertext &);
        seal::Ciphertext add(const seal::Ciphertext &, const seal::Plaintext &);
        seal::Ciphertext add(const seal::Ciphertext &, int);
        seal::Ciphertext add(const seal::Ciphertext &, uint64_t);
        void add_inplace(seal::Ciphertext &, const seal::Ciphertext &);
        void add_inplace(seal::Ciphertext &, const seal::Plaintext &);
        void add_inplace(seal::Ciphertext &, int);
        void add_inplace(seal::Ciphertext &, uint64_t);
        seal::Ciphertext add_many(const std::vector<seal::Ciphertext> &);
        std::vector<uint64_t> decode(const seal::Ciphertext &);
        std::vector<uint64_t> decode(const seal::Plaintext &);
        seal::Plaintext decrypt(const seal::Ciphertext &);
        seal::Plaintext encode(int);
        seal::Plaintext encode(uint64_t);
        seal::Plaintext encode(const std::vector<int> &);
        seal::Plaintext encode(const std::vector<uint64_t> &);
        seal::Ciphertext encrypt(const seal::Plaintext &);
        seal::Ciphertext encrypt(int);
        seal::Ciphertext encrypt(uint64_t);
        seal::Ciphertext encrypt(const std::vector<int> &);
        seal::Ciphertext encrypt(const std::vector<uint64_t> &);
        void generate(int n, int t);
        bool load(const std::string & filename);
        seal::Ciphertext mod_switch_to_next(const seal::Ciphertext &);
        void mod_switch_to_next_inplace(seal::Ciphertext &);
        seal::Ciphertext mul(const seal::Ciphertext &, const seal::Ciphertext &);
        seal::Ciphertext mul(const seal::Ciphertext &, const seal::Plaintext &);
        seal::Ciphertext mul(const seal::Ciphertext &, int);
        seal::Ciphertext mul(const seal::Ciphertext &, uint64_t);
        void mul_inplace(seal::Ciphertext &, const seal::Ciphertext &);
        void mul_inplace(seal::Ciphertext &, const seal::Plaintext &);
        void mul_inplace(seal::Ciphertext &, int);
        void mul_inplace(seal::Ciphertext &, uint64_t);
        seal::Ciphertext mul_many(const std::vector<seal::Ciphertext> &);
        seal::Ciphertext negate(const seal::Ciphertext &);
        void negate_inplace(seal::Ciphertext &);
        int plaintextModulus() const;
        int polynomialDegree() const;
        int reduce(int &);
        void relinearize_inplace(seal::Ciphertext &);
        seal::Ciphertext rescale_mod_switch_to(seal::Ciphertext &, const seal::Ciphertext &);
        void rescale_mod_switch_to_next_inplace(seal::Ciphertext &);
        seal::Ciphertext rotate_columns(const seal::Ciphertext &);
        void rotate_columns_inplace(seal::Ciphertext &);
        seal::Ciphertext rotate_rows(const seal::Ciphertext &, int &);
        void rotate_rows_inplace(seal::Ciphertext &, int &);
        bool save(const std::string & filename) const;
        seal::Ciphertext square(const seal::Ciphertext &);
        void square_inplace(seal::Ciphertext &);
        seal::Ciphertext sub(const seal::Ciphertext &, const seal::Ciphertext &);
        seal::Ciphertext sub(const seal::Ciphertext &, const seal::Plaintext &);
        seal::Ciphertext sub(const seal::Ciphertext &, int);
        seal::Ciphertext sub(const seal::Ciphertext &, uint64_t);
        void sub_inplace(seal::Ciphertext &, const seal::Ciphertext &);
        void sub_inplace(seal::Ciphertext &, const seal::Plaintext &);
        void sub_inplace(seal::Ciphertext &, int);
        void sub_inplace(seal::Ciphertext &, uint64_t);

        static SealBFVKeys loadKeys(const std::string & filename);
        static bool saveKeys(const SealBFVKeys &, const std::string &);
};

} // seal_wrapper
