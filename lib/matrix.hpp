#pragma once

#include "numpy.h"

namespace matrix
{

#if (TEMPLATE == 8)
template <class T> void resize() { T::resizeCache(CACHE_RESIZE); }
template <> inline void resize<int>() {}
template <> inline void resize<unsigned>() {}
template <> inline void resize<int64_t>() {}
template <> inline void resize<uint64_t>() {}
template <> inline void resize<float>() {}
template <> inline void resize<double>() {}
#endif

template <class T, class U> std::vector<std::vector<std::vector<T>>>
conv2d(
    const std::vector<std::vector<std::vector<T>>> & input,
    const std::vector<std::vector<std::vector<std::vector<U>>>> & filters,
    const std::vector<int> & strides
)
{
    auto nChannelsIn  = input.size();
    auto nRowsIn      = input[0].size();
    auto nColsIn      = input[0][0].size();
    auto nChannelsOut = filters.size();
    auto nRowsFilter  = filters[0][0].size();
    auto nColsFilter  = filters[0][0][0].size();
    auto nRowsBorder  = nRowsFilter / 2;
    auto nColsBorder  = nColsFilter / 2;
    if ( nChannelsIn != filters[0].size() ) throw "Incompatible input-filter";
    auto rowStride    = strides[0];
    auto colStride    = strides[1];
    auto nRowsOut     = (nRowsIn - 1*(nRowsFilter-1) - 1 ) / rowStride + 1;
    auto nColsOut     = (nColsIn - 1*(nColsFilter-1) - 1 ) / colStride + 1;

    std::vector<std::vector<std::vector<T>>> output(
        nChannelsOut, std::vector<std::vector<T>>(
            nRowsOut, std::vector<T>( nColsOut )
    ));

    size_t rowOffset = 0;
    // for each row of the output
    for ( size_t io=0; io<nRowsOut; io++ )
    {
        size_t colOffset = 0;
        // for each item of the output's row
        for ( size_t jo=0; jo<nColsOut; jo++ )
        {
            // for each output channel
            for ( size_t co=0; co<nChannelsOut; co++ )
            {
                std::vector<T> partial_res;
                // for each input channel
                for ( size_t ci=0; ci<nChannelsIn; ci++ )
                {
                    // for each row of the filter
                    for ( size_t i=0; i<nRowsFilter; i++ )
                    {
                        // for each item of the filter's row
                        for ( size_t j=0; j<nColsFilter; j++ )
                        {
                            partial_res.push_back( input[ci][i+rowOffset][j+colOffset] * filters[co][ci][i][j] );
                        }
                    }
                }
                output[co][io][jo] = numpy::sum_inplace(partial_res);
            }
            colOffset += colStride;
        }
#if (TEMPLATE==8)
        resize<T>();
#endif
        rowOffset += rowStride;
    }
    return output;
}

template <class T, class U> std::vector<std::vector<std::vector<std::vector<T>>>>
conv2d(
    const std::vector<std::vector<std::vector<std::vector<T>>>> & inputs,
    const std::vector<std::vector<std::vector<std::vector<U>>>> & filters,
    const std::vector<int> & strides)
{
    std::vector<std::vector<std::vector<std::vector<T>>>> v;
    for (const auto & input : inputs) v.push_back( conv2d(input, filters, strides) );
    return v;
}

template <class T, class U> std::vector<std::vector<T>>
matrixMultiplication(const std::vector<std::vector<T>> & a, const std::vector<std::vector<U>> & b)
{
    if ( a.empty() || a[0].empty() || b.empty() || b[0].empty() )
        throw "Matrices must have non-zero dimensions";

    auto n = a.size();
    auto m = a[0].size();
    auto p = b[0].size();

    if ( m != b.size() ) throw "Matrices do not have compatible dimensions";

    std::vector<std::vector<T>> c(n, std::vector<T>(p));
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<p; j++)
        {
            std::vector<T> partial;
            for (int k=0; k<m; k++) partial.push_back( a[i][k] * b[k][j] );
            c[i][j] = numpy::sum_inplace(partial);
        }
    }
    return c;
}

template <class T> std::vector<std::vector<T>>
resize(std::vector<std::vector<T>> & v, int row, int col)
{
    v.resize(row);
    for (auto & e : v) e.resize(col);
    return v;
}

template <class T>
std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>> & v)
{
    if ( v.empty() || v[0].empty() )
        return std::vector<std::vector<T>>();

    std::vector<std::vector<T>> t( v[0].size(), std::vector<T>(v.size()) );
    for (int i=0; i<v.size(); i++)
        for (int j=0; j<v[0].size(); j++)
            t[j][i] = v[i][j];

    return t;
}

} // matrix
