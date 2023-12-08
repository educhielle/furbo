#include "seal_bfv_ciphertext.h"

#include "math.h"
#include "io.h"

using namespace io;
using namespace seal;
using namespace std;

int nativeCopyCounter = 0;

namespace seal_wrapper
{

const int N_COUNTERS = 10;
enum Operation { CT_ADD, CT_MUL, CT_SUB, PT_ADD, PT_MUL, PT_SUB, NEG, SQ, ROT_COL, ROT_ROW };

vector<int> SealBFVCiphertext::counters = vector<int>(N_COUNTERS, 0);
shared_ptr<SealBFVKeys> SealBFVCiphertext::default_keys;
int SealBFVCiphertext::id_counter = 0;
PrintingMode SealBFVCiphertext::printing_mode = PrintingMode::DEFAULT;

SealBFVCiphertext::SealBFVCiphertext()
{
    newId();
}

SealBFVCiphertext::SealBFVCiphertext(int s)
    : SealBFVCiphertext(s, default_keys) {}

SealBFVCiphertext::SealBFVCiphertext(uint64_t s)
    : SealBFVCiphertext(s, default_keys) {}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<int> & v)
    : SealBFVCiphertext(v, default_keys) {}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<uint64_t> & v)
    : SealBFVCiphertext(v, default_keys) {}

SealBFVCiphertext::SealBFVCiphertext(const SealBFVPlaintext & pt)
    : SealBFVCiphertext()
{
    keys = pt.keys;
    ct = keys->encrypt(pt.pt);
}

SealBFVCiphertext::SealBFVCiphertext(const SealBFVCiphertext & ct)
{
    this->keys = ct.keys;
    this->ct = ct.ct;
    this->id = ct.id;
    nativeCopyCounter++;
}

SealBFVCiphertext::SealBFVCiphertext(int s, const SealBFVKeys & keys)
    : SealBFVCiphertext()
{
    this->keys = make_shared<SealBFVKeys>(keys);
    ct = this->keys->encrypt(s);
}

SealBFVCiphertext::SealBFVCiphertext(int s, const std::shared_ptr<SealBFVKeys> & keys)
    : SealBFVCiphertext()
{
    if (!keys) throw "Invalid SealBFVKeys";
    this->keys = keys;
    ct = this->keys->encrypt(s);
}

SealBFVCiphertext::SealBFVCiphertext(uint64_t s, const SealBFVKeys & keys)
    : SealBFVCiphertext()
{
    this->keys = make_shared<SealBFVKeys>(keys);
    ct = this->keys->encrypt(s);
}

SealBFVCiphertext::SealBFVCiphertext(uint64_t s, const std::shared_ptr<SealBFVKeys> & keys)
    : SealBFVCiphertext()
{
    if (!keys) throw "Invalid SealBFVKeys";
    this->keys = keys;
    ct = this->keys->encrypt(s);
}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<int> & v, const SealBFVKeys & keys)
    : SealBFVCiphertext()
{
    this->keys = make_shared<SealBFVKeys>(keys);
    ct = this->keys->encrypt(v);
}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<int> & v, const std::shared_ptr<SealBFVKeys> & keys)
    : SealBFVCiphertext()
{
    if (!keys) throw "Invalid SealBFVKeys";
    this->keys = keys;
    ct = this->keys->encrypt(v);
}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<uint64_t> & v, const SealBFVKeys & keys)
    : SealBFVCiphertext()
{
    this->keys = make_shared<SealBFVKeys>(keys);
    ct = this->keys->encrypt(v);
}

SealBFVCiphertext::SealBFVCiphertext(const std::vector<uint64_t> & v, const std::shared_ptr<SealBFVKeys> & keys)
    : SealBFVCiphertext()
{
    if (!keys) throw "Invalid SealBFVKeys";
    this->keys = keys;
    ct = this->keys->encrypt(v);
}

SealBFVCiphertext::operator int() const
{
    return int( uint64_t(*this) );
}

SealBFVCiphertext::operator uint64_t() const
{
    return decode()[0];
}

SealBFVCiphertext::operator std::vector<int>() const
{
    return decode_int();
}

SealBFVCiphertext::operator std::vector<uint64_t>() const
{
    return decode();
}

SealBFVCiphertext::operator SealBFVPlaintext() const
{
    return decrypt();
}

SealBFVCiphertext::operator string() const
{
    switch (printing_mode)
    {
        case PrintingMode::DEFAULT : return stringify( vector<uint64_t>(*this), PRINTING_MODE_SUMMARY );
        case PrintingMode::ONE : return stringify( int(*this) );
        case PrintingMode::ALL : return stringify( vector<uint64_t>(*this) );
    }
    throw "Unrecognized printing mode (in SealBFVCiphertext)";
}

// compound assignment operators
SealBFVCiphertext & SealBFVCiphertext::operator~()
{
    keys->negate_inplace(ct);
    counters[NEG]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator++()
{
    *this += 1;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator--()
{
    *this -= 1;
    newId();
    return *this;
}

SealBFVCiphertext SealBFVCiphertext::operator++(int)
{
    auto r = *this;
    *this += 1;
    newId();
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator--(int)
{
    auto r = *this;
    *this -= 1;
    newId();
    return r;
}

SealBFVCiphertext & SealBFVCiphertext::operator +=(const SealBFVCiphertext & a)
{
    keys->add_inplace(ct, a.ct);
    counters[CT_ADD]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator *=(const SealBFVCiphertext & a)
{
    if (id == a.id)
    {
        keys->square_inplace(ct);
        counters[SQ]++;
    }
    else
    {
        keys->mul_inplace(ct, a.ct);
        counters[CT_MUL]++;
    }
    keys->relinearize_inplace(ct);
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator -=(const SealBFVCiphertext & a)
{
    keys->sub_inplace(ct, a.ct);
    counters[CT_SUB]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator +=(const SealBFVPlaintext & a)
{
    keys->add_inplace(ct, a.pt);
    counters[PT_ADD]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator *=(const SealBFVPlaintext & a)
{
    keys->mul_inplace(ct, a.pt);
    counters[PT_MUL]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator -=(const SealBFVPlaintext & a)
{
    keys->sub_inplace(ct, a.pt);
    counters[PT_SUB]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator +=(int a)
{
    keys->add_inplace(ct, a);
    counters[PT_ADD]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator *=(int a)
{
    keys->mul_inplace(ct, a);
    counters[PT_MUL]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator -=(int a)
{
    keys->sub_inplace(ct, a);
    counters[PT_SUB]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator ^=(int e)
{
    ct = pow(ct, e);
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator <<=(int s) // rotate rows left
{
    keys->rotate_rows_inplace(ct, s);
    counters[ROT_ROW] += math::ones( math::abs(s) );
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator >>=(int s) // rotate rows right
{
    s = -s;
    keys->rotate_rows_inplace(ct, s);
    counters[ROT_ROW] += math::ones( math::abs(s) );
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator +=(uint64_t a)
{
    keys->add_inplace(ct, a);
    counters[PT_ADD]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator *=(uint64_t a)
{
    keys->mul_inplace(ct, a);
    counters[PT_MUL]++;
    newId();
    return *this;
}

SealBFVCiphertext & SealBFVCiphertext::operator -=(uint64_t a)
{
    keys->sub_inplace(ct, a);
    counters[PT_SUB]++;
    newId();
    return *this;
}

// const operators
SealBFVCiphertext SealBFVCiphertext::operator +() const
{
    return *this;
}

SealBFVCiphertext SealBFVCiphertext::operator -() const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->negate(ct);
    counters[PT_SUB]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator !() const // rotate columns
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->rotate_columns(ct);
    counters[ROT_COL]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator +(const SealBFVCiphertext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->add(ct, a.ct);
    counters[CT_ADD]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator *(const SealBFVCiphertext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    if (id == a.id)
    {
        r.ct = keys->square(ct);
        counters[SQ]++;
    }
    else
    {
        r.ct = keys->mul(ct, a.ct);
        counters[CT_MUL]++;
    }
    keys->relinearize_inplace(r.ct);
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator -(const SealBFVCiphertext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->sub(ct, a.ct);
    counters[CT_SUB]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator +(const SealBFVPlaintext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->add(ct, a.pt);
    counters[PT_ADD]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator *(const SealBFVPlaintext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->mul(ct, a.pt);
    counters[PT_MUL]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator -(const SealBFVPlaintext & a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->sub(ct, a.pt);
    counters[PT_SUB]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator +(int a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->add(ct, a);
    counters[PT_ADD]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator *(int a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->mul(ct, a);
    counters[PT_MUL]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator -(int a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->sub(ct, a);
    counters[PT_SUB]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator ^(int e) const
{
    SealBFVCiphertext r = *this;
    r ^= e;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator <<(int s) const // rotate rows left
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->rotate_rows(ct, s);
    counters[ROT_ROW] += math::ones( math::abs(s) );
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator >>(int s) const // rotate rows right
{
    SealBFVCiphertext r;
    r.keys = keys;
    s = -s;
    r.ct = keys->rotate_rows(ct, s);
    counters[ROT_ROW] += math::ones( math::abs(s) );
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator +(uint64_t a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->add(ct, a);
    counters[PT_ADD]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator *(uint64_t a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->mul(ct, a);
    counters[PT_MUL]++;
    return r;
}

SealBFVCiphertext SealBFVCiphertext::operator -(uint64_t a) const
{
    SealBFVCiphertext r;
    r.keys = keys;
    r.ct = keys->sub(ct, a);
    counters[PT_SUB]++;
    return r;
}

SealBFVCiphertext operator+(const SealBFVPlaintext & a, const SealBFVCiphertext & b)
{
    return b + a;
}

SealBFVCiphertext operator*(const SealBFVPlaintext & a, const SealBFVCiphertext & b)
{
    return b * a;
}

SealBFVCiphertext operator-(const SealBFVPlaintext & a, const SealBFVCiphertext & b)
{
    return -b + a;
}

SealBFVCiphertext operator+(int a, const SealBFVCiphertext & b)
{
    return b + a;
}

SealBFVCiphertext operator*(int a, const SealBFVCiphertext & b)
{
    return b * a;
}

SealBFVCiphertext operator-(int a, const SealBFVCiphertext & b)
{
    return -b + a;
}

SealBFVCiphertext operator+(uint64_t a, const SealBFVCiphertext & b)
{
    return b + a;
}

SealBFVCiphertext operator*(uint64_t a, const SealBFVCiphertext & b)
{
    return b * a;
}

SealBFVCiphertext operator-(uint64_t a, const SealBFVCiphertext & b)
{
    return -b + a;
}

std::vector<uint64_t> SealBFVCiphertext::decode() const
{
    return keys->decode(ct);
}

std::vector<int> SealBFVCiphertext::decode_int() const
{
    auto vu = decode();
    vector<int> vi;
    for (auto & e : vu) vi.push_back( int(e) );
    return vi;
}

SealBFVPlaintext SealBFVCiphertext::decrypt() const
{
    SealBFVPlaintext pt;
    pt.pt = keys->decrypt(ct);
    pt.keys = keys;
    return pt;
}

std::shared_ptr<SealBFVKeys> SealBFVCiphertext::getKeys() const
{
    return keys;
}

void SealBFVCiphertext::newId()
{
    id = id_counter++;
}

int SealBFVCiphertext::plaintextModulus() const
{
    return keys->plaintextModulus();
}

int SealBFVCiphertext::polynomialDegree() const
{
    return keys->polynomialDegree();
}

Ciphertext SealBFVCiphertext::pow(const Ciphertext & b, int e)
{
    if (e < 0) throw "Exponent must be non-negative";
    if (e == 0) return keys->encrypt(1);
    if (e == 1) return b;
    counters[SQ]++;
    auto r = keys->square(b);
    keys->relinearize_inplace(r);
    if (e > 3) r = pow(r, e>>1);
    if (e & 1)
    {
        keys->mul_inplace(r, b);
        keys->relinearize_inplace(r);
        counters[CT_MUL]++;
    }
    return r;
}

SealBFVCiphertext SealBFVCiphertext::add_many(const vector<SealBFVCiphertext> & v)
{
    if ( v.empty() ) return SealBFVCiphertext(0);
    SealBFVCiphertext r;
    r.keys = v[0].keys;
    vector<Ciphertext> vct;
    for (auto & e : v) vct.push_back(e.ct);
    r.ct = r.keys->add_many(vct);
    counters[CT_ADD] += v.size() - 1;
    return r;
}

SealBFVKeys SealBFVCiphertext::defaultKeys(const SealBFVKeys & keys)
{
    default_keys = make_shared<SealBFVKeys>(keys);
    return *default_keys;
}

vector<int> SealBFVCiphertext::getCounters()
{
    return counters;
}

SealBFVCiphertext SealBFVCiphertext::mul_many(const vector<SealBFVCiphertext> & v)
{
    if ( v.empty() ) return SealBFVCiphertext(1);
    SealBFVCiphertext r;
    r.keys = v[0].keys;
    vector<Ciphertext> vct;
    for (auto & e : v) vct.push_back(e.ct);
    r.ct = r.keys->mul_many(vct);
    counters[CT_MUL] += v.size() - 1;
    return r;
}

PrintingMode SealBFVCiphertext::printingMode(const PrintingMode & mode)
{
    printing_mode = mode;
    return printing_mode;
}

ostream & operator <<(ostream & os, const SealBFVCiphertext & a)
{
    return os << string(a);
}

} // seal wrapper

namespace std
{

string to_string(const seal_wrapper::SealBFVCiphertext & ct)
{
    return string(ct);
}

} // std
