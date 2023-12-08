#include "ciphertext.h"
#include "math.h"

namespace smart
{

Ciphertext & Ciphertext::calc(const Ciphertext & a, Operation op)
{
    switch (op)
    {
        case ADD : x += a.x; break;
        case MUL : x *= a.x; break;
        case SUB : x -= a.x; break;
        default : throw "Operator not supported";
    }
    math::reduce(x, t);
    return *this;
}

Ciphertext & Ciphertext::calc(int a, Operation op)
{
    switch (op)
    {
        case ADD : x += a; break;
        case MUL : x *= a; break;
        case SUB : x -= a; break;
        default : throw "Operator not supported";
    }
    math::reduce(x, t);
    return *this;
}

} // smart
