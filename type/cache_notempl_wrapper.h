#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "cache_entry.h"
#include "cache_notempl_manager.h"

namespace seal_wrapper
{
    class SealBFVCiphertext;
}

using Native = seal_wrapper::SealBFVCiphertext;

namespace smart
{

class Cache;

class Wrapper
{
    private:
        int id;
        static Manager manager;
        static Cache cache;
        static std::shared_ptr<Wrapper> p_zero;

    public:
        Wrapper();
        Wrapper(const Native &);
        Wrapper(const Native &, int);
        Wrapper(int);
        Wrapper(const Wrapper &);
        Wrapper(const Wrapper &&);
        ~Wrapper();

        explicit operator Native() const;

        Wrapper & operator=(const Wrapper &);
        Wrapper & operator=(const Wrapper &&) noexcept;

        Wrapper & operator+=(const Wrapper &);
        Wrapper & operator*=(const Wrapper &);
        Wrapper & operator-=(const Wrapper &);
        Wrapper & operator+=(int);
        Wrapper & operator*=(int);
        Wrapper & operator-=(int);

        Wrapper operator+(const Wrapper &) const;
        Wrapper operator*(const Wrapper &) const;
        Wrapper operator-(const Wrapper &) const;
        Wrapper operator+(int) const;
        Wrapper operator*(int) const;
        Wrapper operator-(int) const;

        static void clearCache();
        static void resizeCache(size_t size);
        static void setZero(const Native &);
        static std::vector<int> getCounters();
        static const Cache & getCache() { return cache; }
        static const Manager & getManager() { return manager; }
        int getId() const;

        friend std::ostream & operator <<(std::ostream &, const Wrapper &);
};

inline Wrapper::Wrapper()
    : id(0)
{
    manager.refUp(id);
}

inline Wrapper::Wrapper(const Native & native)
    : id( manager.newId(std::make_shared<Native>(native)) )
{}

inline Wrapper::Wrapper(const Native & native, int id)
    : id( manager.newId(std::make_shared<Native>(native), id) )
{}

inline Wrapper::Wrapper(const Wrapper & a)
    : id(a.id)
{
    manager.refUp(id);
}

inline Wrapper::Wrapper(int aid)
    : id(aid)
{
    manager.refUp(id);
}

inline Wrapper::Wrapper(const Wrapper && a)
    : id(a.id)
{
    manager.refUp(id);
}

inline Wrapper::~Wrapper()
{
    manager.refDown(id);
}

inline Wrapper::operator Native() const
{
    return *manager[id];
}

inline Wrapper & Wrapper::operator =(const Wrapper & a)
{
    if (id != a.id)
    {
        manager.refDown(id);
        id = a.id;
        manager.refUp(id);
    }
    return *this;
}

inline Wrapper & Wrapper::operator =(const Wrapper && a) noexcept
{
    if (id != a.id)
    {
        manager.refDown(id);
        id = a.id;
        manager.refUp(id);
    }
    return *this;
}

} // smart
