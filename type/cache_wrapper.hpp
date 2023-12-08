#pragma once

namespace smart
{

template <class Native>
TypeManager<Native> TypeWrapper<Native>::manager;

template <class Native>
Cache<Entry,TypeWrapper<Native>> TypeWrapper<Native>::cache;

template <class Native>
inline TypeWrapper<Native>::TypeWrapper()
    : id(0)
{
    manager.refUp(id);
}

template <class Native>
inline TypeWrapper<Native>::TypeWrapper(const Native & native)
    : id( manager.newId(native) )
{}

template <class Native>
inline TypeWrapper<Native>::TypeWrapper(const TypeWrapper<Native> & a)
    : id(a.id)
{
    manager.refUp(id);
}

template <class Native>
inline TypeWrapper<Native>::TypeWrapper(const TypeWrapper<Native> && a)
    : id(a.id)
{
    manager.refUp(id);
}

template <class Native>
TypeWrapper<Native>::~TypeWrapper()
{
    manager.refDown(id);
}

template <class Native>
inline TypeWrapper<Native>::operator Native() const
{
    return *manager[id];
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator =(const TypeWrapper<Native> & a)
{
    if (id != a.id)
    {
        manager.refDown(id);
        id = a.id;
        manager.refUp(id);
    }
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator =(const TypeWrapper<Native> && a) noexcept
{
    if (id != a.id)
    {
        manager.refDown(id);
        id = a.id;
        manager.refUp(id);
    }
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator +=(const TypeWrapper<Native> & a)
{
    *this = *this + a;
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator *=(const TypeWrapper<Native> & a)
{
    *this = *this * a;
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator -=(const TypeWrapper<Native> & a)
{
    *this = *this - a;
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator +=(int a)
{
    *this = *this + a;
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator *=(int a)
{
    *this = *this * a;
    return *this;
}

template <class Native>
TypeWrapper<Native> & TypeWrapper<Native>::operator -=(int a)
{
    *this = *this - a;
    return *this;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator +(const TypeWrapper<Native> & a) const
{
    Entry entry(id, a.id, Operator::ADD_CC);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] + *manager[a.id];
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator *(const TypeWrapper<Native> & a) const
{
    Entry entry(id, a.id, Operator::MUL_CC);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] * *manager[a.id];
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator -(const TypeWrapper<Native> & a) const
{
    Entry entry(id, a.id, Operator::SUB_CC);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] - *manager[a.id];
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator +(int a) const
{
    Entry entry(id, a, Operator::ADD_CP);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] + a;
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator *(int a) const
{
    Entry entry(id, a, Operator::MUL_CP);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] * a;
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
TypeWrapper<Native> TypeWrapper<Native>::operator -(int a) const
{
    Entry entry(id, a, Operator::SUB_CP);
    TypeWrapper<Native> ret;
    if ( !cache.get(entry, ret) )
    {
        ret = *manager[id] - a;
        cache.insert(entry, ret);
    }
    return ret;
}

template <class Native>
std::vector<int> TypeWrapper<Native>::getCounters()
{
    return Native::getCounters();
}

template <class Native>
int TypeWrapper<Native>::getId() const
{
    return id;
}


} // smart
