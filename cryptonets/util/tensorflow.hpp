#pragma once

#include <algorithm>
#include <string>
#include <vector>

using std::max;
using std::string;
using std::vector;

#ifndef CACHE_RESIZE
    #define CACHE_RESIZE -1
#endif

template <class T, class U> vector<vector<vector<T>>>
pad(const vector<vector<vector<T>>> & input, const vector<vector<vector<vector<U>>>> & filters, const vector<size_t> & strides)
{
    auto nRowsFilter = filters.size();
    auto nColsFilter = filters[0].size();
    auto nRowsIn = input.size();
    auto nColsIn = input[0].size();
    auto nChannels = input[0][0].size();
    auto nRowsOut = ( nRowsIn + 1 ) / strides[1];
    auto nColsOut = ( nColsIn + 1 ) / strides[2];
    auto padRows = max( (nRowsOut - 1) * strides[1] + nRowsFilter - nRowsIn, size_t(0) );
    auto padCols = max( (nColsOut - 1) * strides[2] + nColsFilter - nColsIn, size_t(0) );
    auto padTop = padRows / 2;
    auto padBottom = padRows - padTop;
    auto padLeft = padCols / 2;
    auto padRight = padCols - padLeft;
    nRowsOut = nRowsIn + padRows;
    nColsOut = nColsIn + padCols;
#ifdef SEAL
    auto zero = T(0,true);
#else
    auto zero = T(0);
#endif
    vector<vector<vector<T>>> output(
        nRowsOut, vector<vector<T>>(
            nColsOut, vector<T>( nChannels, zero ) // zero should be a parameter
    ));
    // for every channel
    for ( size_t k=0; k<nChannels; k++ )
    {
        // copy input to the middle of the output
        for ( size_t i=0; i<nRowsIn; i++ )
            for ( size_t j=0; j<nColsIn; j++ )
                output[i+padTop][j+padLeft][k] = input[i][j][k];
    }
    return output;
}

template <class T, class U> vector<vector<vector<T>>>
conv2d(
    vector<vector<vector<T>>> & input,
    const vector<vector<vector<vector<U>>>> & filters,
    const vector<size_t> & strides
)
{
    auto nRowsIn = input.size();
    auto nColsIn = input[0].size();
    auto nChannelsIn = input[0][0].size();
    auto nRowsFilter = filters.size();
    auto nColsFilter = filters[0].size();
    auto nRowsBorder = nRowsFilter / 2;
    auto nColsBorder = nColsFilter / 2;
    if ( nChannelsIn != filters[0][0].size() ) throw "Incompatible input-filter";
    auto nChannelsOut = filters[0][0][0].size();
    auto rowStride = strides[1];
    auto colStride = strides[2];
    auto nRowsOut = (nRowsIn - 2*nRowsBorder + 1 ) / rowStride;
    auto nColsOut = (nColsIn - 2*nColsBorder + 1 ) / colStride;
    vector<vector<vector<T>>> output(
        nRowsOut, vector<vector<T>>(
            nColsOut, vector<T>( nChannelsOut )
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
                vector<T> partial_res;
                // for each input channel
                for ( size_t ci=0; ci<nChannelsIn; ci++ )
                {
                    // for each row of the filter
                    for ( size_t i=0; i<nRowsFilter; i++ )
                    {
                        // for each item of the filter's row
                        for ( size_t j=0; j<nColsFilter; j++ )
                        {
                            partial_res.push_back( input[i+rowOffset][j+colOffset][ci] * filters[i][j][ci][co] );
                        }
                    }
                }
                output[io][jo][co] = add_vector(partial_res);
            }
            colOffset += colStride;
        }
        for (int k=rowOffset; k<rowOffset+rowStride; k++)
        {
            input[k].clear();
#if (TEMPLATE==8)
            T::resizeCache(CACHE_RESIZE);
#endif
        }
        rowOffset += rowStride;
    }
    return output;
}

template <class T, class U> vector<vector<vector<vector<T>>>>
conv2d_pad_same_nhwc(
    vector<vector<vector<vector<T>>>> & inputs,
    const vector<vector<vector<vector<U>>>> & filters,
    const vector<size_t> & strides
)
{
    vector<vector<vector<vector<T>>>> outputs;
    auto nInputs = inputs.size();
    auto nRows = inputs[0].size();
    auto nCols = inputs[0][0].size();
    auto nChannels = inputs[0][0][0].size();
    // for each input
    for ( size_t i=0; i<nInputs; i++ )
    {
        auto input = pad(inputs[i], filters, strides);
        outputs.push_back( conv2d(input, filters, strides) );
    }
    return outputs;
}

template <class T, class U> vector<vector<vector<vector<T>>>>
conv2d(
    vector<vector<vector<vector<T>>>> & inputs,
    const vector<vector<vector<vector<U>>>> & filters,
    const vector<size_t> & strides,
    const string & padding,
    const string & data_format
)
{
    if ( padding == "SAME" && data_format == "NHWC" ) return conv2d_pad_same_nhwc(inputs, filters, strides);
    throw "Requested mode of conv2d not implemented";
}
