#pragma once

namespace smart
{

template <class Native>
const Native * TypeManager<Native>::operator[](int aid)
{
    return &container[aid].native;
}

template <class Native>
inline void TypeManager<Native>::newEmpty()
{
    container[0] = Value{ 1, Native() };
}

template <class Native>
inline int TypeManager<Native>::newId(const Native & native)
{
    container[++counter] = Value{ 1, native };
    return counter;
}

template <class Native>
inline void TypeManager<Native>::refUp(int id)
{
    container[id].ref++;
}

template <class Native>
inline void TypeManager<Native>::refDown(int id)
{
    auto & ref = container[id].ref;
    if (--ref == 0) container.erase(id);
}

} // smart
