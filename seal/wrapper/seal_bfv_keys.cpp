#include "seal_bfv_keys.h"

#include <fstream>

using namespace seal;
using namespace std;

namespace seal_wrapper
{

SealBFVKeys::SealBFVKeys(int n, int t)
{
    generate(n, t);
}

SealBFVKeys::SealBFVKeys(int n, int t, const SecretKey & sk,
    const PublicKey & pk, const RelinKeys & rk, const GaloisKeys & gk)
{
    this->n = n;
    this->t = t;
    createContext();
    this->sk = sk;
    this->pk = pk;
    this->rk = rk;
    this->gk = gk;
    enc = new Encryptor(*context, pk);
    dec = new Decryptor(*context, sk);
    eval = new Evaluator(*context);
    encoder = new BatchEncoder(*context);
    zero = encrypt(0);
}

SealBFVKeys::SealBFVKeys(const SealBFVKeys & keys)
    : SealBFVKeys(keys.n, keys.t, keys.sk, keys.pk, keys.rk, keys.gk)
{}

SealBFVKeys::~SealBFVKeys()
{
    free(enc);
    free(dec);
    free(eval);
    free(encoder);
}

SealBFVKeys & SealBFVKeys::operator =(const SealBFVKeys & keys)
{
    this->n = keys.n;
    this->t = keys.t;
    createContext();
    this->sk = keys.sk;
    this->pk = keys.pk;
    this->rk = keys.rk;
    this->gk = keys.gk;
    enc = new Encryptor(*context, pk);
    dec = new Decryptor(*context, sk);
    eval = new Evaluator(*context);
    encoder = new BatchEncoder(*context);
    this->zero = keys.zero;
    return *this;
}

Ciphertext SealBFVKeys::add(const Ciphertext & ct1, const Ciphertext & ct2)
{
    Ciphertext cto;
    eval->add(ct1, ct2, cto);
    return cto;
}

Ciphertext SealBFVKeys::add(const Ciphertext & ct, const Plaintext & pt)
{
    Ciphertext cto;
    eval->add_plain(ct, pt, cto);
    return cto;
}

Ciphertext SealBFVKeys::add(const Ciphertext & ct, int s)
{
    return add( ct, encode(s) );
}

Ciphertext SealBFVKeys::add(const Ciphertext & ct, uint64_t s)
{
    return add( ct, encode(s) );
}

void SealBFVKeys::add_inplace(Ciphertext & ct1, const Ciphertext & ct2)
{
    eval->add_inplace(ct1, ct2);
}

void SealBFVKeys::add_inplace(Ciphertext & ct, const Plaintext & pt)
{
    eval->add_plain_inplace(ct, pt);
}

void SealBFVKeys::add_inplace(Ciphertext & ct, int s)
{
    eval->add_plain_inplace( ct, encode(s) );
}

void SealBFVKeys::add_inplace(Ciphertext & ct, uint64_t s)
{
    eval->add_plain_inplace( ct, encode(s) );
}

Ciphertext SealBFVKeys::add_many(const vector<Ciphertext> & vct)
{
    Ciphertext cto;
    eval->add_many(vct, cto);
    return cto;
}

void SealBFVKeys::createContext()
{
    params = EncryptionParameters(scheme_type::bfv);
    params.set_poly_modulus_degree(n);
    params.set_coeff_modulus(CoeffModulus::BFVDefault(n));
    params.set_plain_modulus(t);
    context = make_shared<SEALContext>( SEALContext(params) );
    auto qualifiers = context->first_context_data()->qualifiers();
    if (!qualifiers.using_batching) throw "The plaintext modulus specified does not allow batching";
}

vector<uint64_t> SealBFVKeys::decode(const Plaintext & pt)
{
    vector<uint64_t> v;
    encoder->decode(pt, v);
    return v;
}

vector<uint64_t> SealBFVKeys::decode(const Ciphertext & ct)
{
    return decode( decrypt(ct) );
}

Plaintext SealBFVKeys::decrypt(const Ciphertext & ct)
{
    Plaintext pt;
    dec->decrypt(ct, pt);
    return pt;
}

Plaintext SealBFVKeys::encode(int s)
{
    return encode( vector<uint64_t>( n, uint64_t(reduce(s)) ) );
}

Plaintext SealBFVKeys::encode(uint64_t s)
{
    return encode( vector<uint64_t>(n,s) );
}

Plaintext SealBFVKeys::encode(const vector<int> & v)
{
    Plaintext pt;
    vector<uint64_t> vu;
    for (auto e : v) vu.push_back( uint64_t(reduce(e)) );
    encoder->encode(vu, pt);
    return pt;
}

Plaintext SealBFVKeys::encode(const vector<uint64_t> & v)
{
    Plaintext pt;
    encoder->encode(v, pt);
    return pt;
}

Ciphertext SealBFVKeys::encrypt(const Plaintext & pt)
{
    Ciphertext ct;
    enc->encrypt(pt, ct);
    return ct;
}

Ciphertext SealBFVKeys::encrypt(int s)
{
    return encrypt( encode(s) );
}

Ciphertext SealBFVKeys::encrypt(uint64_t s)
{
    return encrypt( encode(s) );
}

Ciphertext SealBFVKeys::encrypt(const vector<int> & v)
{
    return encrypt( encode(v) );
}

Ciphertext SealBFVKeys::encrypt(const vector<uint64_t> & v)
{
    return encrypt( encode(v) );
}

void SealBFVKeys::generate(int n, int t)
{
    this->n = n;
    this->t = t;
    createContext();
    KeyGenerator keygen(*context);
    sk = keygen.secret_key();
    keygen.create_public_key(pk);
    keygen.create_relin_keys(rk);
    keygen.create_galois_keys(gk);
    enc = new Encryptor(*context, pk);
    dec = new Decryptor(*context, sk);
    eval = new Evaluator(*context);
    encoder = new BatchEncoder(*context);
}

bool SealBFVKeys::load(const string & filename)
{
    string fname;
    // n, t
    try
    {
        fname = filename + ".cfg.key";
        ifstream fin(fname);
        if (fin.fail()) throw "Cannot read '" + fname + "'. Context cannot be created.";
        fin >> n;
        fin >> t;
        createContext();
    }
    catch (...) { throw "Cannot read '" + fname + "'. Context cannot be created."; }

    // secret key
    try
    {
        fname = filename + ".sk.key";
        ifstream fin(fname);
        sk.load(*context, fin);
    }
    catch (...) { std::cout << "WARNING: Cannot read the secret key from '" + fname + "'. Decryption will not work for this key.\n"; }

    // public key
    try
    {
        fname = filename + ".pk.key";
        ifstream fin(fname);
        pk.load(*context, fin);
    }
    catch (...) { std::cout << "WARNING: Cannot read the public key from '" + fname + "'. Encryption will not work for this key.\n"; }

    // relinearization keys
    try
    {
        fname = filename + ".rk.key";
        ifstream fin(fname);
        rk.load(*context, fin);
    }
    catch (...) { std::cout << "WARNING: Cannot read the relinearization keys from '" + fname + "'. Multiplication will not work for this key.\n"; }

    // Galois keys
    try
    {
        fname = filename + ".gk.key";
        ifstream fin(fname);
        gk.load(*context, fin);
    }
    catch (...) { std::cout << "WARNING: Cannot read the Galois keys from '" + fname + "'. Rotations will not work for this key.\n"; }

    return true;
}

SealBFVKeys SealBFVKeys::loadKeys(const string & filename)
{
    SealBFVKeys keys;
    if ( !keys.load(filename) ) throw "Cannot load keys from " + filename;
    return keys;
}

Ciphertext SealBFVKeys::mod_switch_to_next(const Ciphertext & ct)
{
    Ciphertext cto;
    eval->mod_switch_to_next(ct, cto);
    return cto;
}

void SealBFVKeys::mod_switch_to_next_inplace(seal::Ciphertext & ct)
{
    eval->mod_switch_to_next_inplace(ct);
}

Ciphertext SealBFVKeys::mul(const Ciphertext & ct1, const Ciphertext & ct2)
{
    Ciphertext cto;
    eval->multiply(ct1, ct2, cto);
    return cto;
}

Ciphertext SealBFVKeys::mul(const Ciphertext & ct, const Plaintext & pt)
{
    Ciphertext cto;
    eval->multiply_plain(ct, pt, cto);
    return cto;
}

Ciphertext SealBFVKeys::mul(const Ciphertext & ct, int s)
{
    if (!s) return zero;
    return mul( ct, encode(s) );
}

Ciphertext SealBFVKeys::mul(const Ciphertext & ct, uint64_t s)
{
    if (!s) return zero;
    return mul( ct, encode(s) );
}

void SealBFVKeys::mul_inplace(Ciphertext & ct1, const Ciphertext & ct2)
{
    eval->multiply_inplace(ct1, ct2);
}

void SealBFVKeys::mul_inplace(Ciphertext & ct, const Plaintext & pt)
{
    eval->multiply_plain_inplace(ct, pt);
}

void SealBFVKeys::mul_inplace(Ciphertext & ct, int s)
{
    if (!s) ct = zero;
    else eval->multiply_plain_inplace( ct, encode(s) );
}

void SealBFVKeys::mul_inplace(Ciphertext & ct, uint64_t s)
{
    if (!s) ct = zero;
    else eval->multiply_plain_inplace( ct, encode(s) );
}

Ciphertext SealBFVKeys::mul_many(const vector<Ciphertext> & vct)
{
    Ciphertext cto;
    eval->multiply_many(vct, rk, cto);
    return cto;
}

Ciphertext SealBFVKeys::negate(const Ciphertext & ct)
{
    Ciphertext cto;
    eval->negate(ct, cto);
    return cto;
}

void SealBFVKeys::negate_inplace(Ciphertext & ct)
{
    eval->negate_inplace(ct);
}

int SealBFVKeys::plaintextModulus() const
{
    return t;
}

int SealBFVKeys::polynomialDegree() const
{
    return n;
}

int SealBFVKeys::reduce(int & s)
{
    s %= t;
    if (s < 0) s += t;
    return s;
}

void SealBFVKeys::relinearize_inplace(Ciphertext & ct)
{
    eval->relinearize_inplace(ct, rk);
}

Ciphertext SealBFVKeys::rescale_mod_switch_to(Ciphertext & ct1, const Ciphertext & ct2)
{
    if ( ct1.coeff_modulus_size() < ct2.coeff_modulus_size() )
    {
        Ciphertext cto;
        if ( ct2.is_ntt_form() ) eval->mod_switch_to( ct2, ct1.parms_id(), cto );
        else eval->rescale_to( ct2, ct1.parms_id(), cto );
        return cto;
    }

    if ( ct1.coeff_modulus_size() > ct2.coeff_modulus_size() )
    {
        if ( ct1.is_ntt_form() ) eval->mod_switch_to_inplace( ct1, ct2.parms_id() );
        else eval->rescale_to_inplace( ct1, ct2.parms_id() );
    }
    return ct2;
}

void SealBFVKeys::rescale_mod_switch_to_next_inplace(seal::Ciphertext & ct)
{
    if ( ct.is_ntt_form() ) eval->mod_switch_to_next_inplace(ct);
    else eval->rescale_to_next_inplace(ct);
}

Ciphertext SealBFVKeys::rotate_columns(const Ciphertext & ct)
{
    Ciphertext cto;
    eval->rotate_columns(ct, gk, cto);
    return cto;
}

void SealBFVKeys::rotate_columns_inplace(Ciphertext & ct)
{
    eval->rotate_columns_inplace(ct, gk);
}

Ciphertext SealBFVKeys::rotate_rows(const Ciphertext & ct, int & s)
{
    s %= (n >> 1);
    Ciphertext cto;
    eval->rotate_rows(ct, s, gk, cto);
    return cto;
}

void SealBFVKeys::rotate_rows_inplace(Ciphertext & ct, int & s)
{
    s %= (n >> 1);
    eval->rotate_rows_inplace(ct, s, gk);
}

bool SealBFVKeys::saveKeys(const SealBFVKeys & keys, const string & filename)
{
    return keys.save(filename);
}

bool SealBFVKeys::save(const string & filename) const
{
    try
    {
        // n, t
        {
            string fname = filename + ".cfg.key";
            ofstream fout(fname);
            fout << n << '\n';
            fout << t << '\n';
        }
        // secret key
        {
            string fname = filename + ".sk.key";
            ofstream fout(fname);
            sk.save(fout);
        }
        // public key
        {
            string fname = filename + ".pk.key";
            ofstream fout(fname);
            pk.save(fout);
        }
        // relinearization keys
        {
            string fname = filename + ".rk.key";
            ofstream fout(fname);
            rk.save(fout);
        }
        // Galois keys
        {
            string fname = filename + ".gk.key";
            ofstream fout(fname);
            gk.save(fout);
        }
    }
    catch (...) { return false; }
    return true;
}

Ciphertext SealBFVKeys::square(const Ciphertext & ct)
{
    Ciphertext cto;
    eval->square(ct, cto);
    return cto;
}

void SealBFVKeys::square_inplace(Ciphertext & ct)
{
    eval->square_inplace(ct);
}

Ciphertext SealBFVKeys::sub(const Ciphertext & ct1, const Ciphertext & ct2)
{
    Ciphertext cto;
    eval->sub(ct1, ct2, cto);
    return cto;
}

Ciphertext SealBFVKeys::sub(const Ciphertext & ct, const Plaintext & pt)
{
    Ciphertext cto;
    eval->sub_plain(ct, pt, cto);
    return cto;
}

Ciphertext SealBFVKeys::sub(const Ciphertext & ct, int s)
{
    return sub( ct, encode(s) );
}

Ciphertext SealBFVKeys::sub(const Ciphertext & ct, uint64_t s)
{
    return sub( ct, encode(s) );
}

void SealBFVKeys::sub_inplace(Ciphertext & ct1, const Ciphertext & ct2)
{
    eval->sub_inplace(ct1, ct2);
}

void SealBFVKeys::sub_inplace(Ciphertext & ct, const Plaintext & pt)
{
    eval->sub_plain_inplace(ct, pt);
}

void SealBFVKeys::sub_inplace(Ciphertext & ct, int s)
{
    eval->sub_plain_inplace( ct, encode(s) );
}

void SealBFVKeys::sub_inplace(Ciphertext & ct, uint64_t s)
{
    eval->sub_plain_inplace( ct, encode(s) );
}

} // seal_wrapper
