#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

using std::vector;

size_t sizeCeilPowerOfTwo(size_t n)
{
    size_t counter=0;
    while (n)
    {
        counter++;
        n>>=1;
    }
    return counter;
}

template <class T>
T add_vector(vector<T> & v)
{
    auto size = v.size();
    for ( auto n = 1<<sizeCeilPowerOfTwo(size); n>1; )
    {
        n >>= 1;
        for ( size_t i=n; i<size; i++ ) v[i-n] += v[i];
        size = n;
    }
    return v[0];
}

template <class T>
T mul_vector(vector<T> & v)
{
    auto size = v.size();
    for ( auto n = 1<<sizeCeilPowerOfTwo(size); n>1; )
    {
        n >>= 1;
        for ( size_t i=n; i<size; i++ ) v[i-n] *= v[i];
        size = n;
    }
    return v[0];
}

template <class T, class U> vector<vector<T>>
add(const vector<vector<T>> & a, const vector<U> & b)
{
    if ( a[0].size() != b.size() ) throw "Matrix and vector have incompatible sizes";
    vector<vector<T>> c( a.size(), vector<T>(a[0].size()) );
    for ( size_t i=0; i<a.size(); i++ )
        for ( size_t j=0; j<a[0].size(); j++ )
            c[i][j] = a[i][j] + b[j];
    return c;
}

template <class T, class U> vector<vector<vector<vector<T>>>>
add(const vector<vector<vector<vector<T>>>> & a, const vector<U> & b)
{
    auto nItems = a.size();
    auto nRows = a[0].size();
    auto nCols = a[0][0].size();
    auto nChannels = a[0][0][0].size();
    if ( nChannels != b.size() ) throw "Incompatible sizes during addition";
    vector<vector<vector<vector<T>>>> c(
        nItems, vector<vector<vector<T>>>(
            nRows, vector<vector<T>>(
                nCols, vector<T>(nChannels)
    )));
    for ( size_t n=0; n<nItems; n++ )
        for ( size_t i=0; i<nRows; i++ )
            for ( size_t j=0; j<nCols; j++ )
                for ( size_t k=0; k<nChannels; k++ )
                    c[n][i][j][k] = a[n][i][j][k] + b[k];
    return c;
}

template <class T> vector<size_t>
argmax(const vector<vector<T>> & m)
{
    vector<size_t> v;
    for ( size_t i=0; i<m.size(); i++ )
    {
        size_t idx = 0;
        for ( size_t j=0; j<m[0].size(); j++ )
            if ( m[i][j] > m[i][idx] ) idx = j;
        v.push_back(idx);
    }
    return v;
}

template <class T> vector<vector<T>>
combine(const vector<vector<vector<T>>> & a)
{
    vector<vector<T>> r;
    for ( auto & m : a )
    {
        vector<T> vr;
        for ( auto & v : m )
            for ( auto & e : v ) vr.push_back(e);
        r.push_back(vr);
    }
    return r;
}

template <class T> size_t
countEqual(const vector<T> & a, const vector<T> & b)
{
    if ( a.size() != b.size() ) throw "Vectors must have the same size";
    size_t count = 0;
    for ( size_t i=0; i<a.size(); i++ ) count += a[i] == b[i];
    return count;
}

template <class T, class U> vector<vector<T>>
dot(const vector<vector<T>> & a, const vector<vector<U>> & b)
{
    auto n = a.size();
    auto m = a[0].size();
    auto p = b[0].size();
    if ( m != b.size() ) throw "Matrices have incompatible dimensions";
    vector<vector<T>> c( n, vector<T>(p) );
    for ( size_t i=0; i<n; i++ )
    {
        for ( size_t j=0; j<p; j++ )
        {
            vector<T> partial_res;
            for ( size_t k=0; k<m; k++ )
                partial_res.push_back( a[i][k] * b[k][j] );
            c[i][j] = add_vector(partial_res);
        }
    }
    return c;
}

template <class T> T
mean(const vector<T> & a)
{
    vector<T> r;
    std::copy( a.begin(), a.end(), std::back_inserter(r) );
    return add_vector(r);
}

template <class T> T // affects precision
mean(const vector<vector<T>> & a)
{
    vector<T> tmp;
    for ( const auto & v : a )
        for ( const auto & e : v ) tmp.push_back(e);
    return mean(tmp);
}

template <class T> T // affects precision
mean(const vector<vector<vector<T>>> & a)
{
    vector<vector<T>> tmp;
    for ( const auto & v : a )
        for ( const auto & e : v ) tmp.push_back(e);
    return mean(tmp);
}

template <class T> T
mean(const vector<vector<vector<vector<T>>>> & a)
{
    vector<vector<vector<T>>> tmp;
    for ( const auto & v : a )
        for ( const auto & e : v ) tmp.push_back(e);
    return mean(tmp);
}

template <class T> vector<vector<T>>
reshape2d(const vector<vector<vector<vector<T>>>> & a, const vector<size_t> & dimensions)
{
    vector<vector<T>> r;
    for ( const auto & n : a )
    {
        vector<T> tmp;
        for ( const auto & m : n )
            for ( const auto & v : m )
                for ( const auto & e : v )
                    tmp.push_back(e);
        r.push_back(tmp);
    }
    return r;
}

template <class T> vector<vector<vector<vector<T>>>>
reshape4d(const vector<vector<T>> & m, const vector<size_t> & dimensions)
{
    vector<vector<vector<vector<T>>>> r(
        dimensions[0], vector<vector<vector<T>>>(
            dimensions[1], vector<vector<T>>(
                dimensions[2], vector<T>(dimensions[3]))));
    size_t i,j,k,l;
    i=j=k=l=0;
    bool full = false;
    for ( auto & v : m )
    {
        for ( auto & e : v )
        {
            r[i][j][k][l++] = e;
            if (l == dimensions[3])
            {
                l=0;
                k++;
                if (k == dimensions[2])
                {
                    k=0;
                    j++;
                    if (j == dimensions[1])
                    {
                        j=0;
                        i++;
                        if (i == dimensions[0])
                        {
                            full = true;
                            break;
                        }
                    }
                }
            }
        }
        if (full) break;
    }
    return r;
}

template <class T> vector<vector<T>>
reshapeOrder(const vector<vector<T>> & a, const vector<size_t> order)
{
    auto & o = order;
    if ( o.size() != 2 ) throw "order must be of size 2";
    vector<size_t> sizeA = { a.size(), a[0].size() };
    vector<size_t> sizeR;
    for ( auto e : o ) sizeR.push_back( sizeA[e] );
    vector<vector<T>> r( sizeR[0], vector<T>(sizeR[1]) );
    vector<size_t> i = {0,0,0};
    for ( i[0]=0; i[0]<sizeA[0]; i[0]++ )
        for ( i[1]=0; i[1]<sizeA[1]; i[1]++ )
                    r[ i[o[0]] ][ i[o[1]] ] = a[ i[0] ][ i[1] ];
    return r;
}

template <class T> vector<vector<vector<T>>>
reshapeOrder(const vector<vector<vector<T>>> & a, const vector<size_t> order)
{
    auto & o = order;
    if ( o.size() != 3 ) throw "order must be of size 3";
    vector<size_t> sizeA = { a.size(), a[0].size(), a[0][0].size() };
    vector<size_t> sizeR;
    for ( auto e : o ) sizeR.push_back( sizeA[e] );
    vector<vector<vector<T>>> r(
            sizeR[0], vector<vector<T>>(
                sizeR[1], vector<T>(sizeR[2])
    ));
    vector<size_t> i = {0,0,0};
    for ( i[0]=0; i[0]<sizeA[0]; i[0]++ )
        for ( i[1]=0; i[1]<sizeA[1]; i[1]++ )
            for ( i[2]=0; i[2]<sizeA[2]; i[2]++ )
                    r[ i[o[0]] ][ i[o[1]] ][ i[o[2]] ] = a[ i[0] ][ i[1] ][ i[2] ];
    return r;
}

template <class T> vector<vector<vector<vector<T>>>>
reshapeOrder(const vector<vector<vector<vector<T>>>> & a, const vector<size_t> order)
{
    auto & o = order;
    if ( o.size() != 4 ) throw "order must be of size 4";
    vector<size_t> sizeA = { a.size(), a[0].size(), a[0][0].size(), a[0][0][0].size() };
    vector<size_t> sizeR;
    for ( auto e : o ) sizeR.push_back( sizeA[e] );
    vector<vector<vector<vector<T>>>> r(
        sizeR[0], vector<vector<vector<T>>>(
            sizeR[1], vector<vector<T>>(
                sizeR[2], vector<T>(sizeR[3])
    )));
    vector<size_t> i = {0,0,0,0};
    for ( i[0]=0; i[0]<sizeA[0]; i[0]++ )
        for ( i[1]=0; i[1]<sizeA[1]; i[1]++ )
            for ( i[2]=0; i[2]<sizeA[2]; i[2]++ )
                for ( i[3]=0; i[3]<sizeA[3]; i[3]++ )
                    r[ i[o[0]] ][ i[o[1]] ][ i[o[2]] ][ i[o[3]] ] = a[ i[0] ][ i[1] ][ i[2] ][ i[3] ];
    return r;
}

template <class T, class U> T
scale(const U & n, const T & scaler)
{
    return T(n * scaler);
}

template <class T, class U> vector<T>
scale(const vector<U> & n, const T & scaler)
{
    vector<T> r;
    for ( const auto & e : n ) r.push_back( scale(e, scaler) );
    return r;
}

template <class T, class U> vector<vector<T>>
scale(const vector<vector<U>> & n, const T & scaler)
{
    vector<vector<T>> r;
    for ( const auto & e : n ) r.push_back( scale(e, scaler) );
    return r;
}

template <class T, class U> vector<vector<vector<T>>>
scale(const vector<vector<vector<U>>> & n, const T & scaler)
{
    vector<vector<vector<T>>> r;
    for ( const auto & e : n ) r.push_back( scale(e, scaler) );
    return r;
}

template <class T, class U> vector<vector<vector<vector<T>>>>
scale(const vector<vector<vector<vector<U>>>> & n, const T & scaler)
{
    vector<vector<vector<vector<T>>>> r;
    for ( const auto & e : n ) r.push_back( scale(e, scaler) );
    return r;
}

template <class T> vector<size_t>
shape(const vector<T> & a)
{
    vector<size_t> r;
    r.push_back(a.size());
    return r;
}

template <class T> vector<size_t>
shape(const vector<vector<T>> & a)
{
    vector<size_t> r;
    r.push_back(a.size());
    r.push_back(a[0].size());
    return r;
}

template <class T> vector<size_t>
shape(const vector<vector<vector<T>>> & a)
{
    vector<size_t> r;
    r.push_back(a.size());
    r.push_back(a[0].size());
    r.push_back(a[0][0].size());
    return r;
}

template <class T> vector<size_t>
shape(const vector<vector<vector<vector<T>>>> & a)
{
    vector<size_t> r;
    r.push_back(a.size());
    r.push_back(a[0].size());
    r.push_back(a[0][0].size());
    r.push_back(a[0][0][0].size());
    return r;
}

template <class T> T
square(const T & a)
{
    return a * a;
}

template <class T> vector<T>
square(const vector<T> & a)
{
    vector<T> r;
    for ( const auto & e : a ) r.push_back( square(e) );
    return r;
}

template <class T> vector<vector<T>>
square(const vector<vector<T>> & a)
{
    vector<vector<T>> r;
    for ( const auto & e : a ) r.push_back( square(e) );
    return r;
}

template <class T> vector<vector<vector<T>>>
square(const vector<vector<vector<T>>> & a)
{
    vector<vector<vector<T>>> r;
    for ( const auto & e : a ) r.push_back( square(e) );
    return r;
}

template <class T> vector<vector<vector<vector<T>>>>
square(const vector<vector<vector<vector<T>>>> & a)
{
    vector<vector<vector<vector<T>>>> r;
    for (const auto & e : a) r.push_back( square(e) );
    return r;
}

template <class T> T
shift(const T & n, size_t s)
{
    T r = n;
    while (s--) r += r;
    return r;
}

template <class T> vector<T>
shift(const vector<T> & n, size_t s)
{
    vector<T> r;
    for ( const auto & e : n ) r.push_back( shift(e, s) );
    return r;
}

template <class T> vector<vector<T>>
shift(const vector<vector<T>> & n, size_t s)
{
    vector<vector<T>> r;
    for ( const auto & e : n ) r.push_back( shift(e, s) );
    return r;
}

template <class T> vector<vector<vector<T>>>
shift(const vector<vector<vector<T>>> & n, size_t s)
{
    vector<vector<vector<T>>> r;
    for ( const auto & e : n ) r.push_back( shift(e, s) );
    return r;
}

template <class T> vector<vector<vector<vector<T>>>>
shift(const vector<vector<vector<vector<T>>>> & n, size_t s)
{
    vector<vector<vector<vector<T>>>> r;
    for ( const auto & e : n ) r.push_back( shift(e, s) );
    return r;
}

template <class T>
vector<T> subset(const vector<T> & v, size_t begin, size_t end)
{
    vector<T> vss;
    end = v.size() < end ? v.size() : end;
    for ( size_t i=begin; i<end; i++ )
        vss.push_back( v[i] );
    return vss;
}

template <class T> vector<vector<vector<vector<T>>>>
subvector(const vector<vector<vector<vector<T>>>> & x, const vector<size_t> & indices)
{
    vector<vector<vector<vector<T>>>> r;
    for ( size_t i=indices[0]; i<indices[1]; i++ )
    {
        vector<vector<vector<T>>> n;
        for ( size_t j=indices[2]; j<indices[3]; j++ )
        {
            vector<vector<T>> m;
            for ( size_t k=indices[4]; k<indices[5]; k++ )
            {
                vector<T> v;
                for ( size_t l=indices[6]; l<indices[7]; l++ )
                    v.push_back( x[i][j][k][l] );
                m.push_back(v);
            }
            n.push_back(m);
        }
        r.push_back(n);
    }
    return r;
}
