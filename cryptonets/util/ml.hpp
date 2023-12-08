#pragma once

#include <string>
#include <vector>
#include "decryption.hpp"
#include "numpy.hpp"
#include "tensorflow.hpp"
#include "timer.hpp"

// #include <chrono>
// using namespace std::chrono;
// extern nanoseconds ctadd_elapsed;
// extern nanoseconds ctmul_elapsed;
// extern nanoseconds ctsub_elapsed;
// extern nanoseconds ptadd_elapsed;
// extern nanoseconds ptmul_elapsed;
// extern nanoseconds ptsub_elapsed;

#ifdef VECTOR_CLEAR
    #define CLEAR(v) ( v.clear() )
#else
    #define CLEAR(v)
#endif

using std::vector;

void printSmartCounters()
{
#if defined(SMART_DEBUG)
    auto counters = Ciphertext::getCounters();
    for (auto & e : counters) cout << e << ' ';
    cout << '\n';
    Ciphertext::resetCounters();
#endif
}

template <class T> string counters()
{
    string r;
#ifdef USING_CRT
    r = T::getCounters();
    T::resetCounters();
#endif
    return r;
}

template <class Type, class Tensor> void
showInfo(const string & title, Timer & t, const Tensor & tensor, const string & varname)
{
    cout << title << " ( " << t.getSeconds() << " s ) ";
    cout << counters<Type>() << " ";
    cout << varname << " ";
    print(shape(tensor));
    printSmartCounters();
}

template <class T> vector<vector<vector<vector<T>>>>
scaledMeanPool2d(const vector<vector<vector<vector<T>>>> & x, size_t kernel_size)
{
    auto no_of_samples = shape(x)[0];
    auto no_of_channels = shape(x)[3];
    auto sample_size = shape(x)[1];
    auto pool_size = kernel_size;
    auto strides = sample_size / pool_size;
    vector<vector<vector<vector<T>>>> out;
    for (size_t n=0; n<no_of_samples; n++)
    {
        vector<vector<vector<T>>> temp_per_channel(
            strides, vector<vector<T>>(
                strides, vector<T>(no_of_channels)
        ));
        for (size_t c=0; c<no_of_channels; c++)
        {
            for (size_t stride_row=0; stride_row<strides; stride_row++)
            {
                for (size_t stride_col=0; stride_col<strides; stride_col++)
                {
                    auto row_start = stride_row * pool_size;
                    auto col_start = stride_col * pool_size;
                    temp_per_channel[stride_row][stride_col][c] = mean( subvector(x, vector<size_t>{n,n+1, row_start,row_start+pool_size, col_start,col_start+pool_size, c,c+1}) );
                }
            }
        }
        out.push_back(temp_per_channel);
    }
    return out;
}

inline void show_reset_timers()
{
//     std::cout << "   ctadd time = " << ctadd_elapsed.count() << " ns\n";
//     std::cout << "   ctmul time = " << ctmul_elapsed.count() << " ns\n";
//     std::cout << "   ctsub time = " << ctsub_elapsed.count() << " ns\n";
//     std::cout << "   ptadd time = " << ptadd_elapsed.count() << " ns\n";
//     std::cout << "   ptmul time = " << ptmul_elapsed.count() << " ns\n";
//     std::cout << "   ptsub time = " << ptsub_elapsed.count() << " ns\n";
//     std::cout << '\n';
//     ctadd_elapsed = 0ns;
//     ctmul_elapsed = 0ns;
//     ctsub_elapsed = 0ns;
//     ptadd_elapsed = 0ns;
//     ptmul_elapsed = 0ns;
//     ptsub_elapsed = 0ns;
}


template <class T, class U, class V> vector<size_t>
predict(vector<vector<vector<vector<T>>>> & x, const vector<size_t> & y,
    const vector<vector<vector<vector<U>>>> & w1, const vector<V> & b1,
    const vector<vector<vector<vector<U>>>> & w4, const vector<V> & b4,
    const vector<vector<U>> & w8, const vector<V> & b8,
    double & acc)
{
    cout << "xo "; print(shape(x));
    show_reset_timers();

    auto t = Timer();
    auto h1 = add( conv2d(x, w1, vector<size_t>{1,2,2,1}, "SAME", "NHWC"), b1 );
    showInfo<T>("convadd ", t, h1, "h1");
    CLEAR(x);
    show_reset_timers();

    t = Timer();
    auto h2 = square(h1);
    showInfo<T>("square  ", t, h2, "h2");
    CLEAR(h1);
    show_reset_timers();

    t = Timer();
    auto h3 = scaledMeanPool2d(h2, 2);
    showInfo<T>("meanpool", t, h3, "h3");
    CLEAR(h2);
    show_reset_timers();

    t = Timer();
    auto h4 = add( conv2d(h3, w4,  vector<size_t>{1,2,2,1}, "SAME", "NHWC"), b4);
    showInfo<T>("convadd ", t, h4, "h4");
    CLEAR(h3);
    show_reset_timers();

    t = Timer();
    auto h5 = scaledMeanPool2d(h4, 2);
    showInfo<T>("meanpool", t, h5, "h5");
    CLEAR(h4);
    show_reset_timers();

    t = Timer();
    auto h6 = reshape2d( h5, vector<size_t>{h5.size(), h5[0].size() * h5[0][0].size() * h5[0][0][0].size()} );
    showInfo<T>("reshape ", t, h6, "h6");
    CLEAR(h5);
    show_reset_timers();

    t = Timer();
    auto h7 = square(h6);
    showInfo<T>("square  ", t, h7, "h7");
    CLEAR(h6);
    show_reset_timers();

    t = Timer();
    auto h8 = add( dot(h7,w8), b8 );
    showInfo<T>("dotadd  ", t, h8, "h8");
    CLEAR(h7);
    show_reset_timers();

    t = Timer();
#ifdef USING_CRT
    auto h8p = decrypt(h8);
#else
    auto h8p = decrypt<T,U>(h8);
#endif
    showInfo<T>("decrypt ", t, h8, "h8");
    CLEAR(h8);
    show_reset_timers();

    t = Timer();
#ifdef SCHEME_PLAIN
    auto & h8_reshaped2 = h8p;
#else
    auto h8_reshaped = reshapeOrder( h8p, vector<size_t>{1,0,2} );
    cout << "h8_reshaped "; print(shape(h8_reshaped));
    auto h8_combined = combine(h8_reshaped);
    cout << "h8_combined "; print(shape(h8_combined));
    auto h8_reshaped2 = reshapeOrder( h8_combined, vector<size_t>{1,0} );
#endif
    showInfo<T>("plainops", t, h8_reshaped2, "h8_reshaped2");

    auto o = argmax(h8_reshaped2);
    acc = double( countEqual(o, y) ) / o.size();
    return o;
}
