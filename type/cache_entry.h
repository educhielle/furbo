#pragma once

#include <iostream>
#include <unordered_map>

namespace smart
{

enum class Operator { ADD_CC=0, MUL_CC, SUB_CC, ADD_CP, MUL_CP, SUB_CP, SUB_PC };

class Entry
{
    private:
        size_t id;

    public:
        Entry(int, int, Operator);
        bool operator <(const Entry &) const;
        bool operator ==(const Entry &) const;

        friend struct std::hash<Entry>;
};

inline Entry::Entry(int a, int b, Operator o)
{
    id = size_t(o);
    id <<= 30;
    id = a;
    id <<= 30;
    id |= b;
}

inline bool Entry::operator <(const Entry & b) const
{
    return id < b.id;
}

inline bool Entry::operator ==(const Entry & b) const
{
    return id == b.id;
}

} // smart

namespace std
{

template <>
struct hash<smart::Entry>
{
    inline size_t operator()(const smart::Entry & entry) const
    {
        return entry.id;
    }
};

} // std
