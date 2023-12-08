#include "cache_notempl_wrapper.h"
#include "cache_notempl_manager.h"
#include "cache_notempl_cache.h"

#ifndef CTADD
    #define CTADD 1
#endif

#ifndef CTMUL
    #define CTMUL 0
#endif

#ifndef CTSUB
    #define CTSUB 0
#endif

#ifndef PTADD
    #define PTADD 0
#endif

#ifndef PTMUL
    #define PTMUL 1
#endif

#ifndef PTSUB
    #define PTSUB 0
#endif

namespace smart
{

Manager Wrapper::manager;
Cache Wrapper::cache;
std::shared_ptr<Wrapper> Wrapper::p_zero;

Wrapper & Wrapper::operator +=(const Wrapper & a)
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (id == kid)
    {
        *this = a;
        return *this;
    }
    if (a.id == kid) return *this;

#if (CTADD == 1)
    Entry entry(id, a.id, Operator::ADD_CC);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] += *manager[a.id];
        else *this = *manager[id] + *manager[a.id];
#if (CTADD == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper & Wrapper::operator *=(const Wrapper & a)
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (id == kid) return *this;
    if (a.id == kid)
    {
        *this = a;
        return *this;
    }

#if (CTMUL == 1)
    Entry entry(id, a.id, Operator::MUL_CC);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] *= *manager[a.id];
        else *this = *manager[id] * *manager[a.id];
#if (CTMUL == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper & Wrapper::operator -=(const Wrapper & a)
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (a.id == kid) return *this;

#if (CTSUB == 1)
    Entry entry(id, a.id, Operator::SUB_CC);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] -= *manager[a.id];
        else *this = *manager[id] - *manager[a.id];
#if (CTSUB == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper & Wrapper::operator +=(int a)
{
    if (!a) return *this;

#if (PTADD == 1)
    Entry entry(id, a, Operator::ADD_CP);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] += a;
        else *this = *manager[id] + a;
#if (PTADD == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper & Wrapper::operator *=(int a)
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (id == kid) return *this;
    if (!a)
    {
        *this = Wrapper(kid);
        return *this;
    }
    if (a == 1) return *this;

#if (PTMUL == 1)
    Entry entry(id, a, Operator::MUL_CP);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] *= a;
        else *this = *manager[id] * a;
#if (PTMUL == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper & Wrapper::operator -=(int a)
{
    if (!a) return *this;

#if (PTSUB == 1)
    Entry entry(id, a, Operator::SUB_CP);
    if ( !cache.get(entry, *this) )
#endif
    {
        if (manager.nrefs(id) == 1) *manager[id] -= a;
        else *this = *manager[id] - a;
#if (PTSUB == 1)
        cache.insert(entry, *this);
#endif
    }
    return *this;
}

Wrapper Wrapper::operator +(const Wrapper & a) const
{
    int kid = (uint64_t) manager[id]->getKeys().get(); // FIXME e set highest bit to 1
    if (id == kid) return a;
    if (a.id == kid) return *this;

    Wrapper ret;
#if (CTADD == 1)
    Entry entry(id, a.id, Operator::ADD_CC);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] + *manager[a.id];
#if (CTADD == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

Wrapper Wrapper::operator *(const Wrapper & a) const
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (id == kid || a.id == kid) return Wrapper(kid);

    Wrapper ret;
#if (CTMUL == 1)
    Entry entry(id, a.id, Operator::MUL_CC);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] * *manager[a.id];
#if (CTMUL == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

Wrapper Wrapper::operator -(const Wrapper & a) const
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (a.id == kid) return *this;

    Wrapper ret;
#if (CTSUB == 1)
    Entry entry(id, a.id, Operator::SUB_CC);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] - *manager[a.id];
#if (CTSUB == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

Wrapper Wrapper::operator +(int a) const
{
    if (!a) return *this;

    Wrapper ret;
#if (PTADD == 1)
    Entry entry(id, a, Operator::ADD_CP);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] + a;
#if (PTADD == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

Wrapper Wrapper::operator *(int a) const
{
    int kid = (uint64_t) manager[id]->getKeys().get();
    if (!a || id == kid) return Wrapper(kid);
    if (a == 1) return *this;

    Wrapper ret;
#if (PTMUL == 1)
    Entry entry(id, a, Operator::MUL_CP);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] * a;
#if (PTMUL == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

Wrapper Wrapper::operator -(int a) const
{
    if (!a) return *this;

    Wrapper ret;
#if (PTSUB == 1)
    Entry entry(id, a, Operator::SUB_CP);
    if ( !cache.get(entry, ret) )
#endif
    {
        ret = *manager[id] - a;
#if (PTSUB == 1)
        cache.insert(entry, ret);
#endif
    }
    return ret;
}

void Wrapper::clearCache()
{
    cache.clear();
}

std::vector<int> Wrapper::getCounters()
{
    return Native::getCounters();
}

int Wrapper::getId() const
{
    return id;
}

void Wrapper::resizeCache(size_t size)
{
    cache.resize(size);
}

void Wrapper::setZero(const Native & zero)
{
    int kid = (uint64_t) zero.getKeys().get();
    p_zero = std::make_shared<Wrapper>( Wrapper(zero, kid) );
}

} // smart
