#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "crt.hpp"
#include "encryption.hpp"
#include "io.hpp"
#include "ml.hpp"
#include "numpy.hpp"
#include "timer.hpp"

using namespace std;

using Number = __int128;

int main(int argc, char* argv[])
try {
    if ( argc < 9 )
    {
        cout << "Inform the input, weights, biases, and output\n";
        return 1;
    }

    cout << "\nStart\n";
    Timer tStart, t;
    auto inFilename  = string( argv[1] );
    auto w1Filename  = string( argv[2] );
    auto w4Filename  = string( argv[3] );
    auto w8Filename  = string( argv[4] );
    auto b1Filename  = string( argv[5] );
    auto b4Filename  = string( argv[6] );
    auto b8Filename  = string( argv[7] );
    auto outFilename = string( argv[8] );

#if (POLYNOMIAL_DEGREE == 8192)
    const vector<uint64_t> coprimes{65537, 114689, 147457, 163841, 557057};
#else
    const vector<uint64_t> coprimes{65537, 786433, 1179649, 1376257, 1769473};
#endif
    CRT<Number>::setCoprimes(coprimes);
    auto n = CRT<Number>::slots();

    const bool isSubset = true;
    const size_t interval_begin = 0, interval_end = n;
    cout << "slots: " << n << '\n';

    const size_t sbit = 6;
    const size_t ibit = sbit, w1bit = sbit, w4bit = sbit, w8bit = sbit;
    const size_t b1bit = ibit + w1bit;
    const size_t b4bit = 2*b1bit + w4bit + 2;
    const size_t b8bit = 2*b4bit + w8bit + 4;
    const Number scaler_i  = 1 << ibit;
    const Number scaler_w1 = 1 << w1bit;
    const Number scaler_w4 = 1 << w4bit;
    const Number scaler_w8 = 1 << w8bit;
    const Number scaler_b1 = shift( Number(1), size_t(b1bit) );
    const Number scaler_b4 = shift( Number(1), size_t(b4bit) );
    const Number scaler_b8 = shift( Number(1), size_t(b8bit) );
    cout << "\nPrecision/scale:\n";
    cout << " input: " << ibit << '\n';
    cout << " weights: " << w1bit << " " << w4bit << " " << w8bit << '\n';
    cout << " biases: " << b1bit << " " << b4bit << " " << b8bit << '\n';
    cout << '\n';
    cout << "preamble .. ok ( " << t.getSeconds() << " s )\n";

    cout << "load .. " << flush;
    t = Timer();
    auto x0  = load<double>(inFilename, ',');
    auto xp  = reshape4d( x0, vector<size_t>{x0.size(),28,28,1} );
    auto yp  = load<size_t>(outFilename);
    auto w1d = reshape4d( load<double>(w1Filename, ','), vector<size_t>{5,5,1,5} );
    auto b1d = load<double>(b1Filename);
    auto w4d = reshape4d( load<double>(w4Filename, ','), vector<size_t>{5,5,5,10} );
    auto b4d = load<double>(b4Filename);
    auto w8d = load<double>(w8Filename, ',');
    auto b8d = load<double>(b8Filename);
    cout << "ok ( " << t.getSeconds() << " s )\n";

    if ( isSubset )
    {
        xp = subset(xp, interval_begin, interval_end);
        yp = subset(yp, interval_begin, interval_end);
    }

    cout << "scale .. " << flush;
    t = Timer();
    auto xps = scale(xp , scaler_i);
    auto w1s = scale(w1d, scaler_w1);
    auto w4s = scale(w4d, scaler_w4);
    auto w8s = scale(w8d, scaler_w8);
    auto b1s = scale(b1d, scaler_b1);
    auto b4s = scale(b4d, scaler_b4);
    auto b8s = scale(b8d, scaler_b8);
    cout << "ok ( " << t.getSeconds() << " s )\n";

    cout << "encode/encrypt .. " << flush;
    t = Timer();
    auto xc = reshapeOrder( crtEncryptPack( reshapeOrder(xps, vector<size_t>{1,2,3,0}) ), vector<size_t>{3,0,1,2} );
    auto & w1 = w1s;
    auto & w4 = w4s;
    auto & w8 = w8s;
    auto b1 = crtEncode(b1s);
    auto b4 = crtEncode(b4s);
    auto b8 = crtEncode(b8s);
    cout << "ok ( " << t.getSeconds() << " s )\n";

    cout << "\nDimensions:\n";
    cout << "x0 "; print(shape(x0));
    cout << "xp "; print(shape(xp));
    cout << "xc "; print(shape(xc));
    cout << "yp "; print(shape(yp));
    cout << "w1 "; print(shape(w1));
    cout << "b1 "; print(shape(b1));
    cout << "w4 "; print(shape(w4));
    cout << "b4 "; print(shape(b4));
    cout << "w8 "; print(shape(w8));
    cout << "b8 "; print(shape(b8));

    cout << "\nPredict\n";
    t = Timer();
    double acc;
    auto y_test_hat = predict(xc,yp,w1,b1,w4,b4,w8,b8,acc);
    cout << "The accuracy on test data for MNIST: " << acc << " ( " << t.getSeconds() << " s )\n";
    cout << "Done! ( " << tStart.getSeconds() << " s )\n";

    std::cout << "# native copy = " << nativeCopyCounter << '\n';
#if (TEMPLATE==8)
    std::cout << "# ciphertexts = " << smart::Wrapper::getManager().getCounter() << '\n';
    std::cout << "cache entries = " << smart::Wrapper::getCache().size()         << '\n';
    std::cout << " # cache hits = " << smart::Wrapper::getCache().getHits()      << '\n';
    std::cout << " # cache miss = " << smart::Wrapper::getCache().getMisses()    << '\n';
    std::cout << " # cache reqs = " << smart::Wrapper::getCache().getRequests()  << '\n';
#endif

    print( Ciphertext::getCounters() );
}
catch (const string & e) { cout << "Exception: " << e << '\n'; }
catch (const char   * e) { cout << "Exception: " << e << '\n'; }
