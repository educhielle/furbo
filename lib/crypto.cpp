#include "crypto.h"

#include "matrix.h"
#include "numpy.h"

#define IS_TEMPLATE(templ) (templ >= 1)

using namespace matrix;
using namespace std;
using namespace seal_wrapper;

namespace crypto
{

vector<int> counters()
{
    return Ciphertext::getCounters();
}

vector<int> decrypt(const Ciphertext & ct)
{
#if IS_TEMPLATE(TEMPLATE)
    const auto & x = Ct(ct);
#else
    auto & x = ct;
#endif
    return vector<int>(x);
}

vector<int> decrypt(const vector<Ciphertext> & vct)
{
    vector<int> vpt;
    for (auto & ct : vct)
    {
        auto pt = decrypt(ct);
        vpt.insert(vpt.end(), pt.begin(), pt.end());
    }
    return vpt;
}

vector<vector<int>> decrypt(const vector<vector<Ciphertext>> & vx)
{
    auto vct = transpose(vx);
    vector<vector<int>> vpt;
    for (auto & e : vct) vpt.push_back( decrypt(e) );
    return transpose(vpt);
}

vector<vector<int>> decrypt(const vector<vector<Ciphertext>> & vx, int row, int col)
{
    auto vpt = decrypt(vx);
    return resize(vpt, row, col);
}

vector<vector<vector<int>>> decrypt(const vector<vector<vector<Ciphertext>>> & vx)
{
    vector<vector<vector<int>>> vpt;
    for (const auto & v2 : vx)
    {
        vector<vector<int>> tmp2;
        for (const auto & v1 : v2) tmp2.push_back( decrypt(v1) );
        vpt.push_back(tmp2);
    }
    return vpt;
}

vector<vector<vector<vector<int>>>> decrypt(const vector<vector<vector<vector<Ciphertext>>>> & vx)
{
    auto vt = numpy::reshapeOrder(vx, vector<size_t>{1, 2, 3, 0});
    vector<vector<vector<vector<int>>>> vpt;
    for (auto & e : vt) vpt.push_back( decrypt(e) );
    return numpy::reshapeOrder(vpt, vector<size_t>{3, 0, 1, 2});
}

Ciphertext encrypt(int m)
{
    return Ct(m);
}

Ciphertext encrypt(const vector<int> & vm)
{
    return Ct(vm);
}

vector<Ciphertext> encrypt(const vector<int> & vm, int n)
{
    vector<Ciphertext> vx;
    vector<int> vtmp;
    for (auto & e : vm)
    {
        vtmp.push_back(e);
        if ( vtmp.size() == n )
        {
            vx.push_back( encrypt(vtmp) );
            vtmp.clear();
        }
    }
    if ( !vtmp.empty() ) vx.push_back( encrypt(vtmp) );
    return vx;
}

vector<vector<Ciphertext>> encrypt(const vector<vector<int>> & vm, int n)
{
    auto vt = transpose(vm);
    vector<vector<Ciphertext>> vx;
    for (auto & e : vt) vx.push_back( encrypt(e, n) );
    return transpose(vx);
}

vector<vector<vector<vector<Ciphertext>>>> encrypt(
    const vector<vector<vector<vector<int>>>> & vm, int n
)
{
    auto vt = numpy::reshapeOrder(vm, vector<size_t>{1, 2, 3, 0});
    vector<vector<vector<vector<Ciphertext>>>> vx;
    for (auto & v3 : vt)
    {
        vector<vector<vector<Ciphertext>>> tmp3;
        for (auto & v2 : v3)
        {
            vector<vector<Ciphertext>> tmp2;
            for (auto & v1 : v2) tmp2.push_back( encrypt(v1, n) );
            tmp3.push_back(tmp2);
        }
        vx.push_back(tmp3);
    }
    return numpy::reshapeOrder(vx, vector<size_t>{3, 0, 1, 2});
}

void init(int n, int t, int depth)
{
    SealBFVKeys keys(n, t);
    SealBFVPlaintext::defaultKeys(keys);
    SealBFVCiphertext::defaultKeys(keys);
    init_template(t, depth);
}

void init_template(int t, int depth)
{
#if (TEMPLATE >= 1 && TEMPLATE <= 6)
    Ciphertext::defaultModulus(t);
#endif
#if (TEMPLATE >= 1 && TEMPLATE != 7)
    Ciphertext::setZero( Ct(0) );
#endif
#if (TEMPLATE == 4)
    Ciphertext::searchDepth(depth);
#endif
}

} // crypto
