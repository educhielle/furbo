#pragma once

#include <vector>
#include "crt.hpp"

template <class T> vector<T>
decrypt(const CRT<T> & a)
{
    return a.decode();
}

template <class T> vector<vector<T>>
decrypt(const vector<CRT<T>> & a)
{
    vector<vector<T>> r;
    for ( auto & e : a ) r.push_back( decrypt(e) );
    return r;
}

template <class T> vector<vector<vector<T>>>
decrypt(const vector<vector<CRT<T>>> & a)
{
    vector<vector<vector<T>>> r;
    for ( auto & e : a ) r.push_back( decrypt(e) );
    return r;
}

template <class T> vector<vector<vector<vector<T>>>>
decrypt(const vector<vector<vector<CRT<T>>>> & a)
{
    vector<vector<vector<vector<T>>>> r;
    for ( auto & e : a ) r.push_back( decrypt(e) );
    return r;
}

template <class T> vector<vector<vector<vector<vector<T>>>>>
decrypt(const vector<vector<vector<vector<CRT<T>>>>> & a)
{
    vector<vector<vector<vector<vector<T>>>>> r;
    for ( auto & e : a ) r.push_back( decrypt(e) );
    return r;
}
