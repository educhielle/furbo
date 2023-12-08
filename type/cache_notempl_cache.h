#pragma once

#include <unordered_map>
#include "cache_entry.h"
#include "cache_notempl_wrapper.h"

#ifndef MAX_CACHE_SIZE
    #define MAX_CACHE_SIZE -1
#endif

namespace smart
{

class Wrapper;

struct Node
{
    Entry entry;
    Wrapper value;
    Node * prev;
    Node * next;
};

class Cache
{
    private:
        static std::hash<Entry> hash;
        std::unordered_map<Entry, Node>  cache;
        Node * first = nullptr;
        Node * last  = nullptr;
        size_t sizeLimit = MAX_CACHE_SIZE;

        int nhits = 0;
        int nmisses = 0;
        int nrequests = 0;

    public:
        void clear();
        bool get(const Entry &, Wrapper &);
        int getHits() const { return nhits; }
        int getMisses() const { return nmisses; }
        int getRequests() const { return nrequests; }
        void insert(const Entry &, const Wrapper &);
        void resize(size_t=MAX_CACHE_SIZE);
        size_t size() const { return cache.size(); }
};

} // smart
