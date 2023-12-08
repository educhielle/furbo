#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::getline;
using std::ifstream;
using std::stod;
using std::string;
using std::vector;

vector<string> split(const string & s, char delimiter)
{
    vector<string> v;
    string item = "";
    for ( auto & c : s )
    {
        if ( c == delimiter )
        {
            v.push_back(item);
            item = "";
        }
        else item += c;
    }
    v.push_back(item);
    return v;
}

string trim(const string & sin)
{
    string s = sin;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

vector<string> trim(const vector<string> & v)
{
    vector<string> vout;
    for ( auto e : v ) vout.push_back(trim(e));
    return vout;
}

template <class T> vector<T>
load(const string & filename)
{
    vector<T> v;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) ) v.push_back( T( stod(line) ) );
    return v;
}

template <class T> vector<vector<T>>
load(const string & filename, char delimiter)
{
    vector<vector<T>> m;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) )
    {
        auto items = split(line, delimiter);
        vector<T> v;
        for ( auto & item : items ) v.push_back( stod(trim(item)) );
        m.push_back(v);
    }
    return m;
}

template <class T> void
print(const vector<T> & v)
{
    for ( auto & e : v ) cout << e << ' ';
    cout << '\n';
}

template <class T> void
print(const vector<vector<T>> & m)
{
    for ( auto & v : m )
    {
        for ( auto & e : v ) cout << e << ' ';
        cout << '\n';
    }
}

template <class T> void
print(const vector<vector<vector<T>>> & n)
{
    for ( auto & m : n )
    {
        for ( auto & v : m )
        {
            cout << "[ ";
            for ( auto & e : v ) cout << e << ' ';
            cout << "]\n";
        }
        cout << '\n';
    }
}

template <class T> void
print(const vector<vector<vector<vector<T>>>> & a)
{
    for ( auto & n : a )
    {
        for ( auto & m : n )
        {
            cout << "{ ";
            for ( auto & v : m )
            {
                cout << "[ ";
                for ( auto & e : v ) cout << e << ' ' ;
                cout << "] ";
            }
            cout << "} ";
        }
        cout << '\n';
    }
}
