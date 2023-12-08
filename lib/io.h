#pragma once

#include <string>
#include <vector>

namespace io
{

const int SUMMARY_SIZE = 8;

template <class T> void print(const std::vector<T> & v, int maxSize=0);
template <class T> void print(const std::vector<std::vector<T>> & v, int maxSize=0);
template <class T> void printSummary(const T & v);
template <class T> std::string scientificNotation(const T & n);
template <class T> std::string stringify(const T &, int maxSize=0);
template <class T> std::string stringify(const std::vector<T> &, int maxSize=0);
template <class T> std::string stringify(const std::vector<std::vector<T>> &, int maxSize=0);

} // io

#include "io.hpp"
