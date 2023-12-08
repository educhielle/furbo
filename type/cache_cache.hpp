#pragma once

namespace smart
{

template <class Entry, class Value>
bool Cache<Entry,Value>::get(const Entry & entry, Value & returnValue)
{
    auto it = cache.find(entry);
    if ( it == cache.end() ) return false; // not in the cache

    auto & node = it->second;
    returnValue = node.value;
    if (node.prev) node.prev->next = node.next;
    if (node.next) node.next->prev = node.prev;
    last->next = &node;
    node.next = nullptr;
    node.prev = last;
    last = &node;
    return true;
}

template <class Entry, class Value>
void Cache<Entry,Value>::insert(const Entry & entry, const Value & value)
{
    auto it = cache.insert(
        std::pair<Entry,Node<Entry,Value>>(
            entry,
            Node<Entry,Value>{entry, value, last, nullptr}
        )
    ).first; // insert node into the cache and return an iterator
    auto & node = it->second;
    if (last) last->next = &node;
    last = &node;
    if (!first) first = &node;
    resize();
}

template <class Entry, class Value>
void Cache<Entry,Value>::resize()
{
    if ( cache.size() <= sizeLimit ) return;
    auto f = first;
    first = first->next;
    cache.erase(f->entry);
}

} // smart
