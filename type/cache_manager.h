#pragma once

#include <map>

namespace smart
{

template <class Native>
class TypeManager
{
    private:
        struct Value
        {
            int ref;
            Native native;
        };
        std::map<int,Value> container;
        int counter = 0;

    public:
        const Native * operator[](int);

        void newEmpty();
        int  newId(const Native &);
        void refUp(int id);
        void refDown(int id);
};

} // smart

#include "cache_manager.hpp"
