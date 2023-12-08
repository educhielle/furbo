#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
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
    if (argc < 6)
    {
        cout << "Inform the polynomial degree, plaintext modulus, number of rows and columns of the encrypted matrix, number of columns of the plaintext matrix, and [search depth] when using Stateful\n";
        cout << "- The polynomial degree must be a power of two (e.g. 8192)\n";
        cout << "- The plaintext modulus must be a prime that enables batching (e.g. 65537)\n";
        cout << "- The number of rows must be a multiple of the polynomial degree for better performance (e.g. 8192, 16384)\n";
        cout << "- The number of columns of the encrypted matrix can be any positive integer\n";
        cout << "- The number of columns of the plaintext matrix can be any positive integer\n";
        cout << "- The search depth must be greater of equal to two\n";
        return 1;
    }

    const double us2s = 1E-6;

    int n   = stoi( argv[1] );
    int t   = stoi( argv[2] );
    int row = stoi( argv[3] );
    int mid = stoi( argv[4] );
    int col = stoi( argv[5] );
    int dep = argc >= 7 ? stoi( argv[6] ) : 0;
    init(n, t, dep); // initialize static variables used in seal wrapper and smart types

#if (DEBUG==1)
    int maxValue = 1 << ( ( flog2(t) - clog2(mid) ) >> 1 ); // ( ⌊log2(t)⌋ - ⌈log2(mid)⌉ ) / 2
#else
    int maxValue = 1 << ( flog2(t) >> 1 ); // half bits of t, for simplicity
#endif
    cout << "\nMaximum value: " << maxValue << '\n';

    auto a = generateMatrix(row, mid, maxValue);
    cout << "Matrix A: "; print(shape(a));// printSummary(a);
    auto b = generateMatrix(mid, col, maxValue);
    cout << "Matrix B: "; print(shape(b));// printSummary(b);

    auto c = matrixMultiplication(a, b);
    cout << "Matrix C = A x B: "; print(shape(c));// printSummary(c);

    cout << "Encrypting .. " << flush;
    auto x = encrypt(a, n);
    cout << "ok\n";
    cout << "Matrix X = encrypt(A): "; print(shape(x));// printSummary(x);

    cout << "Multiplying matrices .. " << flush;
    high_resolution_clock::time_point timer = high_resolution_clock::now();
    auto y = matrixMultiplication(x, b);
    microseconds elapsed = duration_cast<microseconds>(high_resolution_clock::now() - timer);
    cout << "ok\n";
    cout << "Matrix Y = X x B: "; print(shape(y));// printSummary(y);
    cout << "Time: " << elapsed.count() << " us\n";
    auto runtime = scientificNotation( elapsed.count() * us2s );
    cout << "Time: " << runtime << " s\n";

#if (EVAL==1)
    ofstream fout("runtime.log");
    fout << runtime << '\n';
    fout << stringify( counters() );
#elif (DEBUG==1)
    cout << "Decrypting .. " << flush;
    auto r = decrypt(y, row, col);
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
    // std::cout << " # cache gets = " << smart::Wrapper::getCache().getCounter() << '\n';
#endif

    cout << "Counters: "; print( counters() );
}
catch (const char   * e) { cout << "ERROR: " << e << '\n'; }
catch (const string & e) { cout << "ERROR: " << e << '\n'; }
