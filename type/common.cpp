#include "common.h"

#include <string>
#include "math.h"

using namespace math;

namespace smart
{

int eqsap2(int k)
{
    if (k == 0) return 0;
    if (k < 1) return eqsap2(-k) + 1;
    auto clogk = clog2(k);
    auto lk = (1 << clogk) - k;
    return clogk + ones(lk);
}

int eqadd(int k)
{
    if (k == 0) return 0;
    if (k < 0) return eqadd(-k) + 1;
    return flog2(k) + ones(k) - 1;
}

int eqsaddp2(int k)
{
    return std::min( eqadd(k), eqsap2(k) );
}

} // smart
