#pragma once

#if (TEMPLATE >= 1 && TEMPLATE <= 6)
    #define SMART_CONSTRUCTOR(ct, t, p_zero) ( Ciphertext(ct, t, p_zero) )
#elif (TEMPLATE >= 7)
    #define SMART_CONSTRUCTOR(ct, t, p_zero) ( Ciphertext(ct) )
#else
    #define SMART_CONSTRUCTOR(ct, t, p_zero) ( ct )
#endif


// static variables

template <class Number> vector<uint64_t> CRT<Number>::coprimes;
template <class Number> Number CRT<Number>::mod = 2;
template <class Number> vector<Number> CRT<Number>::nx;
template <class Number> std::shared_ptr<CRT<Number>> CRT<Number>::zero;
template <class Number> vector<std::shared_ptr<Ct>> CRT<Number>::p_zeros;
template <class Number> vector<std::shared_ptr<SealBFVKeys>> CRT<Number>::keys;

template <class Number> size_t CRT<Number>::nAdds;
template <class Number> size_t CRT<Number>::nMulsCC;
template <class Number> size_t CRT<Number>::nMulsCS;
template <class Number> size_t CRT<Number>::nSubs;

template <class T, class U>
inline T reduce(const T & a, const U & mod)
{
    if (!mod) return U(a);
    U r = a % mod;
    if (r < 0) r += mod;
    return r;
}

// constructors

template <class Number>
CRT<Number>::CRT(const CRT<Number> & a)
    : CRT<Number>(a.v)
{}

template <class Number>
CRT<Number>::CRT(const Number & a, bool dummy)
{
    encrypt(a);
}

template <class Number>
CRT<Number>::CRT(const vector<Ciphertext> & a)
{
    v = a;
}

template <class Number>
CRT<Number>::CRT(const vector<Number> & a, bool dummy)
{
    encrypt(a);
}

template <class Number>
CRT<Number>::CRT(const vector<vector<uint64_t>> & a)
{
    encrypt(a);
}

// operators

template <class Number>
CRT<Number> & CRT<Number>::operator+=(const CRT & a)
{
    for (int i=0; i<coprimes.size(); i++)
        v[i] += a.v[i];
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator*=(const CRT & a)
{
    for (int i=0; i<coprimes.size(); i++)
        v[i] *= a.v[i];
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator-=(const CRT & a)
{
    for (int i=0; i<coprimes.size(); i++)
        v[i] -= a.v[i];
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator+=(const Number & a)
{
    auto b = reduce(a, mod);
    for (int i=0; i<coprimes.size(); i++)
        v[i] += (int) reduce(b, coprimes[i]);
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator*=(const Number & a)
{
    auto b = reduce(a, mod);
    for (int i=0; i<coprimes.size(); i++)
        v[i] *= (int) reduce(b, coprimes[i]);
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator-=(const Number & a)
{
    auto b = reduce(a, mod);
    for (int i=0; i<coprimes.size(); i++)
        v[i] -= (int) reduce(b, coprimes[i]);
    return *this;
}

template <class Number>
CRT<Number> CRT<Number>::operator-() const
{
    return *zero - *this;
}

template <class Number>
CRT<Number> CRT<Number>::operator+(const CRT & a) const
{
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] + a.v[i] );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> CRT<Number>::operator*(const CRT & a) const
{
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] * a.v[i] );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> CRT<Number>::operator-(const CRT & a) const
{
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] - a.v[i] );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> CRT<Number>::operator+(const Number & a) const
{
    auto b = reduce(a, mod);
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] + (int) reduce(b, coprimes[i]) );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> CRT<Number>::operator*(const Number & a) const
{
    auto b = reduce(a, mod);
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] * (int) reduce(b, coprimes[i]) );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> CRT<Number>::operator-(const Number & a) const
{
    auto b = reduce(a, mod);
    vector<Ciphertext> vc;
    for (int i=0; i<coprimes.size(); i++)
        vc.push_back( v[i] - (int) reduce(b, coprimes[i]) );
    return CRT<Number>(vc);
}

template <class Number>
CRT<Number> operator+(const Number & a, const CRT<Number> & b)
{
    return b + a;
}

template <class Number>
CRT<Number> operator*(const Number & a, const CRT<Number> & b)
{
    return b * a;
}

template <class Number>
CRT<Number> operator-(const Number & a, const CRT<Number> & b)
{
    return CRT<Number>(a) - b; // not ideal, but this function is not used
}

// functions

template <class Number>
void CRT<Number>::clearCache()
{
#if (TEMPLATE==8)
    Ciphertext::clearCache();
#endif
}

template <class Number>
vector<Number> CRT<Number>::decode(bool sign) const
{
    return CRT<Number>::decode(*this, sign);
}

template <class Number>
vector<Number> CRT<Number>::decode(const CRT<Number> & a, bool sign)
{
    vector<vector<int>> vp;
    for ( const auto & ct : a.v )
        vp.push_back( vector<int>(Ct(ct)) );

    auto size = coprimes.size();
    auto nSlots = slots();
    vector<vector<Number>> vf( nSlots, vector<Number>(size) );
    for ( int i=0; i<size; i++ )
        for ( int j=0; j<nSlots; j++ )
            vf[j][i] = vp[i][j] * nx[i] % mod;

    vector<Number> vr;
    for ( auto & f : vf )
    {
        Number r = 0;
        for ( auto & e : f ) r += e;
        r %= mod;
        r = (sign && r>mod/2) ? r-mod : r;
        vr.push_back(r);
    }

    return vr;
}

template <class Number>
CRT<Number> CRT<Number>::encode(const Number & a)
{
    vector<Number> tmp(slots(), a);
    return encode(tmp);
}

template <class Number>
CRT<Number> CRT<Number>::encode(const vector<Number> & a)
{
    return CRT(a);
}

template <class Number>
void CRT<Number>::encrypt(const Number & a)
{
    vector<Number> tmp(slots(), a);
    encrypt(tmp);
}

template <class Number>
void CRT<Number>::encrypt(const vector<Number> & a)
{
    auto size = coprimes.size();
    vector<vector<uint64_t>> m(size);
    for ( const auto & e : a )
    {
        auto n = e % mod;
        n = n<0 ? mod+n : n;
        for ( int i=0; i<size; i++ )
            m[i].push_back( n % coprimes[i] );
    }
    encrypt(m);
}

template <class Number>
void CRT<Number>::encrypt(const vector<vector<uint64_t>> & a)
{
    for ( int i=0; i<coprimes.size(); i++ )
        v.push_back( SMART_CONSTRUCTOR( Ct(a[i], keys[i]), coprimes[i], p_zeros[i] ) );
}

template <class Number>
vector<uint64_t> CRT<Number>::getCoprimes()
{
    return coprimes;
}

template <class Number>
string CRT<Number>::getCounters()
{
    string r = "[ ";
    r += std::to_string(nAdds  ) + ", ";
    r += std::to_string(nMulsCC) + ", ";
    r += std::to_string(nMulsCS) + ", ";
    r += std::to_string(nSubs  ) + " ]";
    return r;
}

template <class Number>
void CRT<Number>::resetCounters()
{
    nAdds = nMulsCC = nMulsCS = nSubs = 0;
}

template <class Number>
void CRT<Number>::resizeCache(size_t size)
{
#if (TEMPLATE==8)
    Ciphertext::resizeCache(size);
#endif
}

template <class Number>
void CRT<Number>::setCoprimes(const vector<uint64_t> & coprimes, int n)
{
    CRT<Number>::coprimes = coprimes;

    mod = 1;
    keys.clear();
    vector<Ciphertext> vzero;
    for ( const auto & t : coprimes )
    {
        mod *= t;
        auto key = SealBFVKeys(n,t);
        keys.push_back( std::make_shared<SealBFVKeys>(key) );
        auto ct_zero = Ct(0,key);
        p_zeros.push_back( std::make_shared<Ct>(ct_zero) );
#if (TEMPLATE == 8)
        int kid = (uint64_t) keys.back().get();
        vzero.push_back( Ciphertext(ct_zero, kid) );
#else
        vzero.push_back( SMART_CONSTRUCTOR(ct_zero, t, p_zeros.back()) );
        // vzero.push_back( SMART_CONSTRUCTOR(p_zeros.back(), t, p_zeros.back()) );
#endif
    }
    zero = std::make_shared<CRT<Number>>(vzero);

    nx.clear();
    for ( const auto & t : coprimes )
    {
        Number ni = mod / t;
        Number nxi;
        for ( uint64_t xi=0; (nxi = xi * ni) % t != 1; xi++ );
        nx.push_back(nxi);
    }
}

template <class Number>
size_t CRT<Number>::slots()
{
    return keys[0]->polynomialDegree();
}

template <class Number>
string CRT<Number>::str() const
{
    string s = std::to_string( int( Ct(v[0]) ) );
    for ( int i=1; i<v.size(); i++)
        s += " " + std::to_string( int( Ct(v[i]) ) );

    // string s = std::to_string(v[0]);
    // for ( int i=1; i<v.size(); i++)
    //     s += " " + std::to_string(v[i]);
    return s;
}

template <class Number>
std::ostream & operator<<(std::ostream & os, const CRT<Number> & a)
{
    return os << a.str();
}
