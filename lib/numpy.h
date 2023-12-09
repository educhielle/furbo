#pragma once

#include <vector>

using std::size_t;

namespace numpy
{

template <class T> bool
operator ==(const std::vector<T> &, const std::vector<T> &);

template <class T, class U> std::vector<std::vector<T>>
add(const std::vector<std::vector<T>> &, const std::vector<U> &);

template <class T, class U> std::vector<std::vector<std::vector<std::vector<T>>>>
add(const std::vector<std::vector<std::vector<std::vector<T>>>> &, const std::vector<U> &);

template <class T> std::vector<size_t> argmax(const std::vector<std::vector<T>> &);

template <class T> std::vector<std::vector<T>>
combine(const std::vector<std::vector<std::vector<T>>> &);

template <class T> size_t
countEqual(const std::vector<T> &, const std::vector<T> &);

template <class T, class U> std::vector<std::vector<T>>
dot(const std::vector<std::vector<T>> &, const std::vector<std::vector<U>> &);

template <class T> T mean(const std::vector<T> &);

template <class T> T mean(const std::vector<std::vector<T>> &);

template <class T> T mean(const std::vector<std::vector<std::vector<T>>> &);

template <class T> T mean(const std::vector<std::vector<std::vector<std::vector<T>>>> &);

template <class T> T product_inplace(std::vector<T> &);

template <class T> std::vector<std::vector<T>>
reshape2d(const std::vector<std::vector<std::vector<std::vector<T>>>> &, const std::vector<size_t> & dimensions);

template <class T> std::vector<std::vector<std::vector<std::vector<T>>>>
reshape4d(const std::vector<std::vector<T>> &, const std::vector<size_t> & dimensions);

template <class T> std::vector<std::vector<T>>
reshapeOrder(const std::vector<std::vector<T>> &, const std::vector<size_t> order);

template <class T> std::vector<std::vector<std::vector<T>>>
reshapeOrder(const std::vector<std::vector<std::vector<T>>> &, const std::vector<size_t> order);

template <class T> std::vector<std::vector<std::vector<std::vector<T>>>>
reshapeOrder(const std::vector<std::vector<std::vector<std::vector<T>>>> &, const std::vector<size_t> order);

template <class T, class U> T scale(const U &, const T & scaler);

template <class T, class U> std::vector<T> scale(const std::vector<U> &, const T & scaler);

template <class T, class U> std::vector<std::vector<T>>
scale(const std::vector<std::vector<U>> &, const T & scaler);

template <class T, class U> std::vector<std::vector<std::vector<T>>>
scale(const std::vector<std::vector<std::vector<U>>> &, const T & scaler);

template <class T, class U> std::vector<std::vector<std::vector<std::vector<T>>>>
scale(const std::vector<std::vector<std::vector<std::vector<U>>>> &, const T & scaler);

template <class T> std::vector<size_t> shape(const std::vector<T> &);

template <class T> std::vector<size_t> shape(const std::vector<std::vector<T>> &);

template <class T> std::vector<size_t> shape(const std::vector<std::vector<std::vector<T>>> &);

template <class T> std::vector<size_t> shape(const std::vector<std::vector<std::vector<std::vector<T>>>> &);

template <class T> T square(const T &);

template <class T> std::vector<T> square(const std::vector<T> &);

template <class T> std::vector<std::vector<T>> square(const std::vector<std::vector<T>> &);

template <class T> std::vector<std::vector<std::vector<T>>> square(const std::vector<std::vector<std::vector<T>>> &);

template <class T> std::vector<std::vector<std::vector<std::vector<T>>>>
square(const std::vector<std::vector<std::vector<std::vector<T>>>> &);

template <class T> T shift(const T &, size_t s);

template <class T> std::vector<T> shift(const std::vector<T> &, size_t s);

template <class T> std::vector<std::vector<T>> shift(const std::vector<std::vector<T>> &, size_t s);

template <class T> std::vector<std::vector<std::vector<T>>>
shift(const std::vector<std::vector<std::vector<T>>> &, size_t s);

template <class T> std::vector<std::vector<std::vector<std::vector<T>>>>
shift(const std::vector<std::vector<std::vector<std::vector<T>>>> &, size_t s);

template <class T> std::vector<T> subset(const std::vector<T> & v, size_t begin, size_t end);

template <class T> std::vector<std::vector<std::vector<std::vector<T>>>>
subvector(const std::vector<std::vector<std::vector<std::vector<T>>>> & x, const std::vector<size_t> & indices);

template <class T> T sum(const std::vector<T> & v);

template <class T> T sum_inplace(std::vector<T> & v);

} // numpy

#include "numpy.hpp"
