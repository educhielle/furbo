#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "common.h"
#include "crypto.h"
#include "io.h" // debug
#include "math.h"
#include "matrix.h"
#include "numpy.h"

using namespace crypto;
using namespace io; // debug
using namespace math;
using namespace matrix;
using namespace numpy;
using namespace smart;
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
try
{
    if (argc < 12)
    {
        cout << "Inform the polynomial degree, plaintext modulus, "
            << "number of batches, output and input channels, rows, and columns of the encrypted input, "
            << "the number of rows and columns of the kernel, and the row and column strides\n";
        cout << "- The polynomial degree must be a power of two (e.g. 8192)\n";
        cout << "- The plaintext modulus must be a prime that enables batching (e.g. 65537)\n";
        cout << "- The number of batches must be a multiple of the polynomial degree for better performance (e.g. 8192, 16384)\n";
        cout << "- The number of output channels can be any positive integer (recommended values: 1 or 3)\n";
        cout << "- The number of input channels can be any positive integer (recommended values: 1 or 3)\n";
        cout << "- The number of rows of the encrypted input can be any positive integer\n";
        cout << "- The number of columns of the encrypted input can be any positive integer\n";
        cout << "- The number of rows of the kernel can be any small positive integer (recommended: 3)\n";
        cout << "- The number of columns of the kernel can be any small positive integer (recommended: 3)\n";
        cout << "- The row stride can be any small positive integer (recommended: 1 or 2)\n";
        cout << "- The column stride can be any small positive integer (recommended: 1 or 2)\n";
        return 1;
    }

    const double us2s = 1E-6;

    int n   = stoi( argv[ 1] );
    int t   = stoi( argv[ 2] );
    int bat = stoi( argv[ 3] ); // number of batches
    int cho = stoi( argv[ 4] );
    int chi = stoi( argv[ 5] );
    int row = stoi( argv[ 6] );
    int col = stoi( argv[ 7] );
    int fro = stoi( argv[ 8] );
    int fco = stoi( argv[ 9] );
    int str = stoi (argv[10] );
    int stc = stoi (argv[11] );
    init(n, t); // initialize static variables used in seal wrapper and smart types

#if (DEBUG==1)
    int maxValue = 1 << ( ( flog2(t) - clog2(col) ) >> 1 ); // ( ⌊log2(t)⌋ - ⌈log2(mid)⌉ ) / 2
#else
    int maxValue = 1 << ( flog2(t) >> 1 ); // half bits of t, for simplicity
#endif
    cout << "\nMaximum value: " << maxValue << '\n';

    auto a = generateTensor4D(bat, chi, row, col, maxValue);
    cout << "Inputs: "; print(shape(a));// printSummary(a);
    auto b = generateTensor4D(cho, chi, fro, fco, maxValue);
    cout << "Filter: "; print(shape(b));// printSummary(b);
    vector<int> strides{str, stc};
    cout << "Strides: "; print(strides);

    auto c = conv2d(a, b, strides);
    cout << "Tensor C = conv(A, B): "; print(shape(c));// printSummary(c);

    cout << "Encrypting .. " << flush;
    auto x = encrypt(a, n);
    cout << "ok\n";
    cout << "Tensor X = encrypt(A): "; print(shape(x));// printSummary(x);

    cout << "Convolving .. " << flush;
    high_resolution_clock::time_point timer = high_resolution_clock::now();
    auto y = conv2d(x, b, strides);
    microseconds elapsed = duration_cast<microseconds>(high_resolution_clock::now() - timer);
    cout << "ok\n";
    cout << "Tensor Y = conv(X, B): "; print(shape(y));// printSummary(y);
    cout << "Time: " << elapsed.count() << " us\n";
    auto runtime = scientificNotation( elapsed.count() * us2s );
    cout << "Time: " << runtime << " s\n";

#if (EVAL==1)
    ofstream fout("runtime.log");
    fout << runtime << '\n';
    fout << stringify( counters() );
#elif (DEBUG==1)
    cout << "Decrypting .. " << flush;
    auto r = decrypt(y);
    cout << "ok\n";
    cout << "\nMatrix R = dec(Y): "; print(shape(r)); printSummary(r);
    cout << (r == c ? "Result is correct\n" : "Wrong result\n");
#endif

    std::cout << "# native copy = " << nativeCopyCounter << '\n';
#if (TEMPLATE==8)
    std::cout << "# ciphertexts = " << smart::Wrapper::getManager().getCounter() << '\n';
    std::cout << "cache entries = " << smart::Wrapper::getCache().size()         << '\n';
    std::cout << " # cache hits = " << smart::Wrapper::getCache().getHits()      << '\n';
    std::cout << " # cache miss = " << smart::Wrapper::getCache().getMisses()    << '\n';
    std::cout << " # cache reqs = " << smart::Wrapper::getCache().getRequests()  << '\n';
#endif

    cout << "Counters: "; print( counters() );
}
catch (const char   * e) { cout << "ERROR: " << e << '\n'; }
catch (const string & e) { cout << "ERROR: " << e << '\n'; }
