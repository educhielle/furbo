#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "common.h"

template <class T>
std::string decrypt(const T & ct)
{
    std::ostringstream os;
    os << ct;
    return os.str();
}

template <class T, class U>
T operate(const T & a, const U & b, smart::Operation op)
{
    switch (op)
    {
        case smart::ADD : return a + b;
        case smart::MUL : return a * b;
        case smart::SUB : return a - b;
        default: throw "Invalid operation";
    }
}

template <class T, class U>
void test(const std::vector<T> & va, const std::vector<U> & vb)
{
    const char vop[] = {'+', '*', '-'};
    for (int op=0; op<smart::N_OPERATIONS; op++)
        for (auto & c0 : va)
            for (auto & c1 : vb)
                std::cout << decrypt(c0) << ' '
                    << decrypt(vop[op]) << ' '
                    << decrypt(c1) << " = "
                    << decrypt(operate(c0, c1, smart::Operation(op))) << '\n';
}
