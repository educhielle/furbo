#pragma once

#include <vector>
#include "crt.hpp"

template <class T> CRT<T>
crtEncode(const T & a)
{
    return CRT<T>(a);
}

template <class T> vector<CRT<T>>
crtEncode(const vector<T> & a)
{
    vector<CRT<T>> r;
    for ( auto & e : a ) r.push_back( crtEncode(e) );
    return r;
}

template <class T> vector<vector<CRT<T>>>
crtEncode(const vector<vector<T>> & a)
{
    vector<vector<CRT<T>>> r;
    for ( auto & e : a ) r.push_back( crtEncode(e) );
    return r;
}

template <class T> vector<vector<vector<CRT<T>>>>
crtEncode(const vector<vector<vector<T>>> & a)
{
    vector<vector<vector<CRT<T>>>> r;
    for ( auto & e : a ) r.push_back( crtEncode(e) );
    return r;
}

template <class T> vector<vector<vector<vector<CRT<T>>>>>
crtEncode(const vector<vector<vector<vector<T>>>> & a)
{
    vector<vector<vector<vector<CRT<T>>>>> r;
    for ( auto & e : a ) r.push_back( crtEncode(e) );
    return r;
}

template <class T> CRT<T>
crtEncrypt(const T & a)
{
    return CRT<T>(a, true);
}

template <class T> vector<CRT<T>>
crtEncrypt(const vector<T> & a)
{
    vector<CRT<T>> r;
    for ( auto & e : a ) r.push_back( crtEncrypt(e) );
    return r;
}

template <class T> vector<vector<CRT<T>>>
crtEncrypt(const vector<vector<T>> & a)
{
    vector<vector<CRT<T>>> r;
    for ( auto & e : a ) r.push_back( crtEncrypt(e) );
    return r;
}

template <class T> vector<vector<vector<CRT<T>>>>
crtEncrypt(const vector<vector<vector<T>>> & a)
{
    vector<vector<vector<CRT<T>>>> r;
    for ( auto & e : a ) r.push_back( crtEncrypt(e) );
    return r;
}

template <class T> vector<vector<vector<vector<CRT<T>>>>>
crtEncrypt(const vector<vector<vector<vector<T>>>> & a)
{
    vector<vector<vector<vector<CRT<T>>>>> r;
    for ( auto & e : a ) r.push_back( crtEncrypt(e) );
    return r;
}

template <class T> vector<CRT<T>>
crtEncryptPack(const vector<T> & a)
{
    vector<T> v;
    v.assign(a.begin(), a.end());
    auto nElems = a.size();
    auto slots = CRT<T>::slots();
    // auto mod = nElems % slots;
    auto mod = (slots - nElems) % slots; // changed from 'mod = nElems % slots;'
    v.resize(v.size()+mod, 0);
    vector<CRT<T>> r;
    for ( size_t i=0; i<v.size(); i+=slots )
    {
        vector<T> tmp;
        for ( size_t j=0; j<slots; j++ )
            tmp.push_back(v[i+j]);
        r.push_back( CRT<T>(tmp, true) );
    }
    return r;
}

template <class T> vector<vector<CRT<T>>>
crtEncryptPack(const vector<vector<T>> & a)
{
    vector<vector<CRT<T>>> r;
    for ( auto & e : a ) r.push_back( crtEncryptPack(e) );
    return r;
}

template <class T> vector<vector<vector<CRT<T>>>>
crtEncryptPack(const vector<vector<vector<T>>> & a)
{
    vector<vector<vector<CRT<T>>>> r;
    for ( auto & e : a ) r.push_back( crtEncryptPack(e) );
    return r;
}

template <class T> vector<vector<vector<vector<CRT<T>>>>>
crtEncryptPack(const vector<vector<vector<vector<T>>>> & a)
{
    vector<vector<vector<vector<CRT<T>>>>> r;
    for ( auto & e : a ) r.push_back( crtEncryptPack(e) );
    return r;
}
