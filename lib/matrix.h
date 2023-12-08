#pragma once

#include <vector>

namespace matrix
{

std::vector<std::vector<int>> generateMatrix(int nRows, int nColumns, int maxValue);

std::vector<std::vector<std::vector<int>>>
generateTensor3D(int nChannels, int nRows, int nColumns, int maxValue);

std::vector<std::vector<std::vector<std::vector<int>>>>
generateTensor4D(int nInputs, int nChannels, int nRows, int nColumns, int maxValue);

template <class T, class U> std::vector<std::vector<std::vector<T>>>
conv2d(const std::vector<std::vector<std::vector<T>>> &, const std::vector<std::vector<std::vector<std::vector<U>>>> &, const std::vector<int> &);

template <class T, class U> std::vector<std::vector<std::vector<std::vector<T>>>>
conv2d(const std::vector<std::vector<std::vector<std::vector<T>>>> &, const std::vector<std::vector<std::vector<std::vector<U>>>> &, const std::vector<int> &);

template <class T, class U> std::vector<std::vector<T>>
matrixMultiplication(const std::vector<std::vector<T>> &, const std::vector<std::vector<U>> &);

template <class T> std::vector<std::vector<T>>
resize(std::vector<std::vector<T>> &, int row, int col);

template <class T>
std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>> &);

} // matrix

#include "matrix.hpp"
