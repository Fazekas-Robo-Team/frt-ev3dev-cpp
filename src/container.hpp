#pragma once

#include <memory_resource>
#include <vector>
#include <mutex>

#define USING_TYPES \
    using typename Container<Data>::value_type; \
    using typename Container<Data>::size_type; \
    using typename Container<Data>::iterator; \
    using typename Container<Data>::const_iterator; \
    using typename Container<Data>::reverse_iterator; \
    using typename Container<Data>::const_reverse_iterator; \
    using typename Container<Data>::reference; \
    using typename Container<Data>::const_reference; \

namespace FRT
{

template <typename Data>
class Container
{
    protected:
        Data data;
        mutable std::recursive_mutex mutex;

    public:
        using value_type = typename Data::value_type;
        using size_type = typename Data::size_type;
        using iterator = typename Data::iterator;
        using const_iterator = typename Data::const_iterator;
        using reverse_iterator = typename Data::reverse_iterator;
        using const_reverse_iterator = typename Data::const_reverse_iterator;
        using reference = typename Data::reference;
        using const_reference = typename Data::const_reference;
        
        Container () {};

        Container (const Container &container) 
        {
            const auto lock = std::scoped_lock(container.mutex);
            data = container.data;
        }

        Container (Container &&container) 
        {
            const auto lock = std::scoped_lock(container.mutex);
            data = std::move(container.data);
        }

        Container (const Data &data) : data(data) {}

        Container (Data &&data) : data(std::move(data)) {}

        template <typename... Args> Container (Args &...args) : data(args...) {}

        Container (std::initializer_list<typename Data::value_type> args) : data(args) {}

        constexpr void lock () { mutex.lock(); }

        constexpr void unlock () { mutex.unlock(); }

        constexpr iterator begin () { return data.begin(); }

        constexpr iterator end () { return data.end(); }

        constexpr const_iterator cbegin () const { return data.cbegin(); }

        constexpr const_iterator cend () const { return data.cend(); }

        constexpr bool empty () const { return data.empty(); }

        constexpr size_type max_size () const { return data.max_size(); }

        constexpr void clear ()
        {
            const auto lock = std::scoped_lock(mutex);
            data.clear();
        }

        // TODO: expand to functions with standard conforming signatures
        template <typename... Args> 
        constexpr iterator insert (Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return data.insert(args...);
        }

        template <typename... Args> 
        constexpr iterator emplace (const_iterator pos, Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return data.emplace(pos, args...);
        }

        // TODO: expand to functions with standard conforming signatures
        template <typename... Args> 
        constexpr iterator erase (Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return data.erase(args...);
        }

        constexpr void swap (Container<Data> &other)
        {
            const auto lock = std::scoped_lock(mutex, other.mutex);
            data.swap(other->data);
        }
};

template <typename Data>
std::ostream &operator<< (std::ostream &stream, const Container<Data> &container) 
{ 
    const auto lock = std::scoped_lock(container.mutex);
    stream << "{ "; 
    std::string sep; 
    for (const auto &x : container.data) {
        stream << sep << x, sep = ", "; 
    }
    return stream << " }"; 
}

template <typename Data>
class SequenceContainer : public Container<Data>
{
    public:
        using Container<Data>::Container;
        USING_TYPES

        reverse_iterator rbegin () { return this->data.rbegin(); }

        reverse_iterator rend () { return this->data.rend(); }

        const_reverse_iterator crbegin () const { return this->data.crbegin(); }

        const_reverse_iterator crend () const { return this->data.crend(); }

        constexpr void resize (size_type count)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.resize(count);
        }

        constexpr void resize (size_type count, const value_type &value)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.resize(count, value);
        }

        constexpr void assign (size_type count, const value_type &value)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.assign(count, value);
        }

        template <typename InputIt> 
        constexpr void assign (InputIt first, InputIt last)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.assign(first, last);
        }

        constexpr void assign (std::initializer_list<value_type> ilist)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.assign(ilist);
        }

        constexpr void push_back (const value_type &value)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.push_back(value);
        }

        constexpr void push_back (value_type &&value)
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.push_back(std::move(value));
        }

        constexpr void pop_back ()
        {
            const auto lock = std::scoped_lock(this->mutex);
            this->data.pop_back();
        }

        template <typename... Args> 
        constexpr reference emplace_back (Args &&...args)
        {
            const auto lock = std::scoped_lock(this->mutex);
            return this->data.emplace_back(args...);
        }


};

template <typename T>
class Vector : public SequenceContainer<std::pmr::vector<T>>
{
    public:
        using Data = std::pmr::vector<T>;
        using SequenceContainer<std::pmr::vector<T>>::SequenceContainer;
        USING_TYPES

        constexpr reference at (size_type pos) { return Container<Data>::data.at(pos); }

        constexpr const_reference at (size_type pos) const { return Container<Data>::data.at(pos); }

        constexpr reference operator[] (size_type pos) { return Container<Data>::data[pos]; }

        constexpr const_reference operator[] (size_type pos) const { return Container<Data>::data[pos]; }

        constexpr value_type *data () { return Container<Data>::data(); }

        constexpr const value_type *data () const { return Container<Data>::data.data(); }

        constexpr size_type capacity () const { return Container<Data>::data.capacity(); }

        constexpr void reserve (size_type new_size)
        {
            const auto lock = std::scoped_lock(this->mutex);
            Container<Data>::data.reserve(new_size);
        }
};

/*
    TODO: 
    pmr::deque, pmr::list inherited from SequenceContainer
    base class AssociativeContainer
    pmr::set, pmr::map inherited from AssociativeContainer
    base class UnorderedAssociativeContainer
    pmr::unordered_set, pmr::unordered_map inherited from UnorderedAssociativeContainer
*/

} // namespace
