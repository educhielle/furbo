#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "cache_cache.h"
#include "cache_entry.h"
#include "cache_manager.h"

namespace smart
{

template <class Native>
class TypeWrapper
{
    private:
        int id;
        static TypeManager<Native> manager;
        static Cache<Entry,TypeWrapper<Native>> cache;

    public:
        TypeWrapper();
        TypeWrapper(const Native &);
        TypeWrapper(const TypeWrapper &);
        TypeWrapper(const TypeWrapper &&);
        ~TypeWrapper();

        explicit operator Native() const;

        TypeWrapper & operator=(const TypeWrapper &);
        TypeWrapper & operator=(const TypeWrapper &&) noexcept;

        TypeWrapper & operator+=(const TypeWrapper &);
        TypeWrapper & operator*=(const TypeWrapper &);
        TypeWrapper & operator-=(const TypeWrapper &);
        TypeWrapper & operator+=(int);
        TypeWrapper & operator*=(int);
        TypeWrapper & operator-=(int);

        TypeWrapper operator+(const TypeWrapper &) const;
        TypeWrapper operator*(const TypeWrapper &) const;
        TypeWrapper operator-(const TypeWrapper &) const;
        TypeWrapper operator+(int) const;
        TypeWrapper operator*(int) const;
        TypeWrapper operator-(int) const;

        static std::vector<int> getCounters();
        int getId() const;

        template <class T>
        friend std::ostream & operator <<(std::ostream &, const TypeWrapper<T> &);
};

} // smart

#include "cache_wrapper.hpp"
