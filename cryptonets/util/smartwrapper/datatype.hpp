#pragma once

#include "seal_bfv.h"

#if (TEMPLATE == 1)
    #include "logadder.h"
#elif (TEMPLATE == 2)
    #include "stateless.h"
#elif (TEMPLATE == 3)
    #include "partialstate.h"
#elif (TEMPLATE == 4)
    #include "stateful.h"
#elif (TEMPLATE == 5)
    #include "memorymul.h"
#elif (TEMPLATE == 6)
    #include "memorytype.h"
#elif (TEMPLATE == 7)
    #include "cache.h"
#elif (TEMPLATE == 8)
    #include "cache_notempl.h"
#endif

namespace crypto
{

using Ct = seal_wrapper::SealBFVCiphertext;

#if (TEMPLATE == 1)
    using Ciphertext = smart::LogAdder<Ct>;
#elif (TEMPLATE == 2)
    using Ciphertext = smart::Stateless<Ct>;
#elif (TEMPLATE == 3)
    using Ciphertext = smart::PartialState<Ct>;
#elif (TEMPLATE == 4)
    using Ciphertext = smart::Stateful<Ct>;
#elif (TEMPLATE == 5)
    using Ciphertext = smart::MemoryMul<Ct>;
#elif (TEMPLATE == 6)
    using Ciphertext = smart::MemoryType<Ct>;
#elif (TEMPLATE == 7)
    using Ciphertext = smart::TypeWrapper<Ct>;
#elif (TEMPLATE == 8)
    using Ciphertext = smart::Wrapper;
#else
    using Ciphertext = Ct;
#endif

} // crypto
