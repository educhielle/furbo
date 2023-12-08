#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace smart
{

template <class T>
class Pool
{
    private:
        std::shared_ptr<std::vector<std::shared_ptr<T>>> pool;

    public:
        Pool();
        Pool(const T &);
        Pool(const std::vector<T> &);
        Pool(const std::vector<std::shared_ptr<T>> &);
        Pool(const std::shared_ptr<std::vector<std::shared_ptr<T>>> &);
        Pool(const Pool &);

        std::shared_ptr<T> & operator [](int);
        const std::shared_ptr<T> & operator [](int) const;

        std::shared_ptr<T> & back();
        void clear();
        void push_back(const T &);
        void push_back(const std::shared_ptr<T> &);
        size_t size() const;

        template <class U>
        friend std::ostream & operator<<(std::ostream &, const Pool<U> &);
};

} // smart

#include "pool_shared.hpp"
