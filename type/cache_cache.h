#pragma once

#include <map>

#ifndef MAX_CACHE_SIZE
    #define MAX_CACHE_SIZE -1
#endif

namespace smart
{

template <class Entry, class Value>
struct Node
{
    Entry entry;
    Value value;
    Node * prev;
    Node * next;
};

template <class Entry, class Value>
class Cache
{
    private:
        std::map<Entry, Node<Entry,Value>> cache;
        Node<Entry,Value> * first = nullptr;
        Node<Entry,Value> * last  = nullptr;
        size_t sizeLimit = MAX_CACHE_SIZE;

    public:

        bool get(const Entry &, Value &);
        void insert(const Entry &, const Value &);
        void resize();
};

} // smart

#include "cache_cache.hpp"
