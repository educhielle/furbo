#pragma once

#include <string>
#include <vector>
#include "smartwrapper/datatype.hpp"

#ifndef POLYNOMIAL_DEGREE
    #define POLYNOMIAL_DEGREE 8192
#endif

using namespace crypto;
using namespace seal_wrapper;
using std::string;
using std::vector;

template <class Number>
class CRT
{
    private:
        vector<Ciphertext> v;

        static vector<uint64_t> coprimes;
        static Number mod;
        static vector<Number> nx;
        static std::shared_ptr<CRT<Number>> zero;
        static vector<std::shared_ptr<Ct>> p_zeros;
        static vector<std::shared_ptr<SealBFVKeys>> keys;

        // statistics
        static size_t nAdds;
        static size_t nMulsCC;
        static size_t nMulsCS;
        static size_t nSubs;

        void encrypt(const Number &);
        void encrypt(const vector<Number> &);
        void encrypt(const vector<vector<uint64_t>> &);

    public:
        CRT() : CRT( Number(0) ) {}
        CRT(const CRT &);
        CRT(const Number &, bool=false);
        CRT(const vector<Ciphertext> &);
        CRT(const vector<Number> &, bool=false);
        CRT(const vector<vector<uint64_t>> &);

        CRT & operator+=(const CRT &);
        CRT & operator*=(const CRT &);
        CRT & operator-=(const CRT &);
        CRT & operator+=(const Number &);
        CRT & operator*=(const Number &);
        CRT & operator-=(const Number &);

        CRT operator-() const;
        CRT operator+(const CRT &) const;
        CRT operator*(const CRT &) const;
        CRT operator-(const CRT &) const;
        CRT operator+(const Number &) const;
        CRT operator*(const Number &) const;
        CRT operator-(const Number &) const;

        template <class T> friend CRT operator+(const T &, const CRT<T> &);
        template <class T> friend CRT operator*(const T &, const CRT<T> &);
        template <class T> friend CRT operator-(const T &, const CRT<T> &);

        vector<Number> decode(bool sign=true) const;
        static void clearCache();
        static vector<Number> decode(const CRT &, bool sign=true);
        static CRT encode(const Number &);
        static CRT encode(const vector<Number> &);
        static vector<uint64_t> getCoprimes();
        static string getCounters();
        static void resetCounters();
        static void resizeCache(size_t size);
        static void setCoprimes(const vector<uint64_t> &, int n=POLYNOMIAL_DEGREE);
        static size_t slots();
        string str() const;

        template <class T> friend std::ostream & operator<<(std::ostream &, const CRT<T> &);
};

#include "crt_smartwrapper.hpp"
