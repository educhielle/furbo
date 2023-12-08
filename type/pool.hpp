#pragma once

namespace smart
{

template <class T>
Pool<T>::Pool(const T & a)
{
    this->push_back(a);
}

template <class T>
Pool<T>::Pool(const std::vector<T> & v)
{
    for (auto & e : v) this->push_back(e);
}

template <class T>
Pool<T>::Pool(const std::vector<std::shared_ptr<T>> & pool)
{
    this->pool = pool;
}

template <class T>
Pool<T>::Pool(const Pool<T> & pool) : Pool(pool.pool) {}

template <class T> inline std::shared_ptr<T> &
Pool<T>::operator [](int a)
{
    if ( a < 0 ) throw "The index must be non negative. Got " + std::to_string(a);
    if ( a >= pool.size() ) pool.resize( a+1, nullptr );
    return pool[a];
}

template <class T> const inline std::shared_ptr<T> &
Pool<T>::operator [](int a) const
{
    if ( a < 0 ) throw "The index must be non negative. Got " + std::to_string(a);
    if ( a >= pool.size() ) throw "Out of range";
    return pool[a];
}

template <class T> std::shared_ptr<T> &
Pool<T>::back()
{
    return pool.back();
}

template <class T> void
Pool<T>::clear()
{
    pool.clear();
}

template <class T> void
Pool<T>::push_back(const std::shared_ptr<T> & a)
{
    pool.push_back(a);
}

template <class T> void
Pool<T>::push_back(const T & a)
{
    this->push_back(std::make_shared<T>(a));
}

template <class T> size_t
Pool<T>::size() const
{
    return pool.size();
}

template <class U> std::ostream &
operator<<(std::ostream & os, const Pool<U> & a)
{
    os << "{ ";
    for (auto & e : a.pool)
    {
        if (e == nullptr) os << "null ";
        else os << *e << ' ';
    }
    os << '}';
    return os;
}

} // smart
