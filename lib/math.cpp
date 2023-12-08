#include "math.h"

#include <string>

namespace math
{

int abs(int n)
{
    return n < 0 ? -n : n;
}

int clog2(int k)
{
    auto l = flog2(k);
    return k == 1<<l ? l : l+1;
}

int flog2(int k)
{
    if (k <= 0) throw "Cannot calculate the logarithm of non-positive numbers ( " + std::to_string(k) + " )";
    auto l = 0;
    while (k >>= 1) l++;
    return l;
}

int msb(int n)
{
    int msb = 0;
    for (; n; n >>= 1 ) msb++;
    return msb;
}

int ones(int k)
{
    int n = 0;
    while (k)
    {
        n += k & 1;
        k >>= 1;
    }
    return n;
}

void reduce(int & a, int mod)
{
    if (!mod) return;
    a %= mod;
    a = a < 0 ? mod+a : a;
}

} // math
