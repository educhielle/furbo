#pragma once

#include <iostream>

namespace io
{

template <class T>
void print(const std::vector<T> & v, int maxSize)
{
    std::cout << stringify(v, maxSize) << '\n';
}

template <class T> void print(const std::vector<std::vector<T>> & v, int maxSize)
{
    std::cout << stringify(v, maxSize) << '\n';
}

template <class T> void printSummary(const T & v)
{
    print(v, SUMMARY_SIZE);
}

template <class T> std::string scientificNotation(const T & n)
{
    double m = n;
    int e = 0;
    while (m > 10)
    {
        m /= 10;
        e++;
    }
    return std::to_string(m) + "E+" + std::to_string(e);
}

template <class T> std::string stringify(const T & a, int maxSize)
{
    return std::to_string(a);
}

template <class T> std::string stringify(const std::vector<T> & v, int maxSize,
    const std::string & begin, const std::string & end, const std::string & spacing,
    const std::string & elemSep, const std::string & etc)
{
    const int half = maxSize >> 1;
    std::string s = begin + spacing;
    auto elemSpacing = elemSep + spacing;
    if (maxSize && v.size() > maxSize)
    {
        for ( int i=0; i<half; i++ ) s += stringify(v[i], maxSize) + elemSpacing;
        s.append(etc);
        for ( int i=v.size()-half; i<v.size(); i++ )
            s += stringify(v[i], maxSize) + (i == v.size()-1 ? spacing : elemSpacing);
    }
    else
    {
        for ( int i=0; i<v.size(); i++ )
            s += stringify(v[i], maxSize) + (i == v.size()-1 ? spacing : elemSpacing);
    }
    s.append(end);
    return s;
}

template <class T> std::string stringify(const std::vector<T> & v, int maxSize)
{
    return stringify(v, maxSize, "{", "}", " ", "", " ... ");
}

template <class T> std::string stringify(const std::vector<std::vector<T>> & v, int maxSize)
{
    return stringify(v, maxSize, "{\n ", "\n}", "", ",\n ", " ...\n ");
}

} // io
