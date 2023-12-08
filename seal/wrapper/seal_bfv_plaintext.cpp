#include "seal_bfv_plaintext.h"

#include <iostream>

using namespace std;

namespace seal_wrapper
{

shared_ptr<SealBFVKeys> SealBFVPlaintext::default_keys;

SealBFVPlaintext::SealBFVPlaintext(int s)
    : SealBFVPlaintext(s, *default_keys) {}

SealBFVPlaintext::SealBFVPlaintext(uint64_t s)
    : SealBFVPlaintext(s, *default_keys) {}

SealBFVPlaintext::SealBFVPlaintext(const std::vector<int> & v)
    : SealBFVPlaintext(v, *default_keys) {}

SealBFVPlaintext::SealBFVPlaintext(const std::vector<uint64_t> & v)
    : SealBFVPlaintext(v, *default_keys) {}

SealBFVPlaintext::SealBFVPlaintext(const SealBFVPlaintext & pt)
{
    this->keys = pt.keys;
    this->pt = pt.pt;
}

SealBFVPlaintext::SealBFVPlaintext(int s, const SealBFVKeys & keys)
{
    this->keys = make_shared<SealBFVKeys>(keys);
    pt = this->keys->encode(s);
}

SealBFVPlaintext::SealBFVPlaintext(uint64_t s, const SealBFVKeys & keys)
{
    this->keys = make_shared<SealBFVKeys>(keys);
    pt = this->keys->encode(s);
}

SealBFVPlaintext::SealBFVPlaintext(const std::vector<int> & s, const SealBFVKeys & keys)
{
    this->keys = make_shared<SealBFVKeys>(keys);
    pt = this->keys->encode(s);
}

SealBFVPlaintext::SealBFVPlaintext(const std::vector<uint64_t> & s, const SealBFVKeys & keys)
{
    this->keys = make_shared<SealBFVKeys>(keys);
    pt = this->keys->encode(s);
}

SealBFVPlaintext::operator int()
{
    return int( uint64_t(*this) );
}

SealBFVPlaintext::operator uint64_t()
{
    return decode()[0];
}

SealBFVPlaintext::operator vector<int>()
{
    return decode_int();
}

SealBFVPlaintext::operator vector<uint64_t>()
{
    return decode();
}

vector<uint64_t> SealBFVPlaintext::decode() const
{
    return keys->decode(pt);
}

vector<int> SealBFVPlaintext::decode_int() const
{
    auto vu = decode();
    vector<int> vi;
    for (auto & e : vu) vi.push_back( int(e) );
    return vi;
}

int SealBFVPlaintext::plaintextModulus() const
{
    return keys->plaintextModulus();
}

int SealBFVPlaintext::polynomialDegree() const
{
    return keys->polynomialDegree();
}

SealBFVKeys SealBFVPlaintext::defaultKeys(const SealBFVKeys & keys)
{
    default_keys = make_shared<SealBFVKeys>(keys);
    return *default_keys;
}

} // seal_wrapper
