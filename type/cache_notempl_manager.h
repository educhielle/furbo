#pragma once

#include <memory>
#include <unordered_map>
#include "seal_bfv.h"

using Native = seal_wrapper::SealBFVCiphertext;

namespace smart
{

class Manager
{
    private:
        struct Value
        {
            int ref;
            std::shared_ptr<Native> native;
        };
        std::unordered_map<int,Value> container;
        int counter = 0;

    public:
        const std::shared_ptr<Native> operator[](int);

        void newEmpty();
        int  newId(const std::shared_ptr<Native> &);
        int  newId(const std::shared_ptr<Native> &, int);
        int  nrefs(int id);
        void refUp(int id);
        void refDown(int id);
        int getCounter() const { return counter; }
};

inline const std::shared_ptr<Native> Manager::operator[](int aid)
{
    return container[aid].native;
}

inline void Manager::newEmpty()
{
    container[0] = Value{ 1, nullptr };
}

inline int Manager::newId(const std::shared_ptr<Native> & native)
{
    container[++counter] = Value{ 1, native };
    return counter;
}

inline int Manager::newId(const std::shared_ptr<Native> & native, int id)
{
    container[id] = Value{ 1, native };
    return id;
}

inline int Manager::nrefs(int id)
{
    return container[id].ref;
}

inline void Manager::refUp(int id)
{
    container[id].ref++;
}

inline void Manager::refDown(int id)
{
    auto & ref = container[id].ref;
    if (--ref == 0) container.erase(id);
}

} // smart
