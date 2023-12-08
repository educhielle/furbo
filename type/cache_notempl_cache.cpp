#include "cache_notempl_cache.h"

#include <iostream>
#include <utility>

#ifndef LRU
    #define LRU 0
#endif

namespace smart
{

std::hash<Entry> Cache::hash;

void Cache::clear()
{
    cache.clear();
}

bool Cache::get(const Entry & entry, Wrapper & returnValue)
{
    nrequests++;
    auto it = cache.find(entry);
    if ( it == cache.end() )
    {
        nmisses++;
        return false; // not in the cache
    }

    auto & node = it->second;
    returnValue = node.value;
#if (LRU==1)
    if (node.prev) node.prev->next = node.next;
    else first = node.next;
    if (node.next) node.next->prev = node.prev;
    last->next = &node;
    node.next = nullptr;
    node.prev = last;
    last = &node;
#endif
    nhits++;
    return true;
}

void Cache::insert(const Entry & entry, const Wrapper & value)
{
    auto it = cache.emplace(
        std::pair<Entry,Node>{
            entry,
            Node{entry, value, last, nullptr}
        }
    ).first; // insert node into the cache and return an iterator
    auto & node = it->second;
    if (last) last->next = &node;
    last = &node;
    if (!first) first = &node;
    resize(sizeLimit);
}

void Cache::resize(size_t size)
{
    while ( cache.size() > size )
    {
        auto f = first;
        first->next->prev = nullptr;
        first = first->next;
        cache.erase(f->entry);
    }
}

} // smart
