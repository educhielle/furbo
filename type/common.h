#pragma once

#include <vector>

namespace smart
{

enum Operation { ADD, MUL, SUB };
const int N_OPERATIONS = 3;
const int N_OP_TYPES   = 2;

int eqadd(int k);
int eqsaddp2(int k);
int eqsap2(int k);

} // smart
