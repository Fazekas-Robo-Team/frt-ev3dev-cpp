#pragma once

#include <memory_resource>
#include <mutex>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#define USING_TYPES \
    using typename Container<Data>::value_type; \
    using typename Container<Data>::size_type; \
    using typename Container<Data>::iterator; \
    using typename Container<Data>::const_iterator; \
    using typename Container<Data>::reverse_iterator; \
    using typename Container<Data>::const_reverse_iterator; \
    using typename Container<Data>::reference; \
    using typename Container<Data>::const_reference; \
    using typename Container<Data>::allocator_type; \
    using typename Container<Data>::difference_type; \
    using typename Container<Data>::pointer;

namespace FRT
{

template <typename Data>
class Container
{
    protected:
        Data container;
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
        using allocator_type = typename Data::allocator_type;
        using difference_type = typename Data::difference_type;
        using pointer = typename Data::pointer;
        
        Container () {};

        Container (const Container<Data> &other) 
        {
            const auto lock = std::scoped_lock(other.mutex);
            container = other.container;
        }

        Container (Container<Data> &&other) 
        {
            const auto lock = std::scoped_lock(other.mutex);
            container = std::move(other.container);
        }

        Container (const Data &data) : container(data) {}

        Container (Data &&data) : container(std::move(data)) {}

        template <typename... Args> Container (Args &...args) : container(args...) {}

        Container (std::initializer_list<value_type> args) : container(args) {}

        constexpr void lock () { mutex.lock(); }

        constexpr void unlock () { mutex.unlock(); }

        friend std::ostream &operator<< (std::ostream &stream, const Container<Data> &container) 
        { 
            const auto lock = std::scoped_lock(container.mutex);
            stream << "{ "; 
            std::string separator; 
            for (const auto &x : container.container) {
                stream << separator << x; 
                separator = ", ";
            }
            return stream << " }"; 
        }

        constexpr allocator_type get_allocator () const { return container.get_allocator(); }

        constexpr iterator begin () { return container.begin(); }

        constexpr iterator end () { return container.end(); }

        constexpr const_iterator cbegin () const { return container.cbegin(); }

        constexpr const_iterator cend () const { return container.cend(); }

        constexpr bool empty () const { return container.empty(); }

        constexpr size_type max_size () const { return container.max_size(); }

        constexpr void clear ()
        {
            const auto lock = std::scoped_lock(mutex);
            container.clear();
        }

        // TODO: expand to functions with standard conforming signatures
        template <typename... Args> 
        constexpr iterator insert (Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return container.insert(args...);
        }

        template <typename... Args> 
        constexpr iterator emplace (const_iterator pos, Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return container.emplace(pos, args...);
        }

        // TODO: expand to functions with standard conforming signatures
        template <typename... Args> 
        constexpr iterator erase (Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return container.erase(args...);
        }

        constexpr void swap (Container<Data> &other)
        {
            const auto lock = std::scoped_lock(mutex, other.mutex);
            container.swap(other->container);
        }

    protected:
        reverse_iterator rbegin () { return container.rbegin(); }

        reverse_iterator rend () { return container.rend(); }

        const_reverse_iterator crbegin () const { return container.crbegin(); }

        const_reverse_iterator crend () const { return container.crend(); }

        constexpr reference at (size_type pos) { return container.at(pos); }

        constexpr const_reference at (size_type pos) const { return container.at(pos); }

        constexpr reference operator[] (size_type pos) { return container[pos]; }

        constexpr const_reference operator[] (size_type pos) const { return container[pos]; }

        constexpr value_type *data () { return container.data(); }

        constexpr const value_type *data () const { return container.data(); }

        constexpr size_type capacity () const { return container.capacity(); }

        constexpr void resize (size_type count)
        {
            const auto lock = std::scoped_lock(mutex);
            container.resize(count);
        }

        constexpr void resize (size_type count, const value_type &value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.resize(count, value);
        }

        constexpr void assign (size_type count, const value_type &value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.assign(count, value);
        }

        template <typename InputIt> 
        constexpr void assign (InputIt first, InputIt last)
        {
            const auto lock = std::scoped_lock(mutex);
            container.assign(first, last);
        }

        constexpr void assign (std::initializer_list<value_type> ilist)
        {
            const auto lock = std::scoped_lock(mutex);
            container.assign(ilist);
        }

        constexpr void push_back (const value_type &value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.push_back(value);
        }

        constexpr void push_back (value_type &&value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.push_back(std::move(value));
        }

        constexpr void pop_back ()
        {
            const auto lock = std::scoped_lock(mutex);
            container.pop_back();
        }

        template <typename... Args> 
        constexpr reference emplace_back (Args &&...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return container.emplace_back(args...);
        }

        constexpr void reserve (size_type new_size)
        {
            const auto lock = std::scoped_lock(mutex);
            container.reserve(new_size);
        }

        constexpr void shrink_to_fit ()
        {
            const auto lock = std::scoped_lock(mutex);
            container.shrink_to_fit();
        }

        constexpr void push_front (const value_type &value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.push_front(value);
        }

        constexpr void push_front (value_type &&value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.push_front(std::move(value));
        }

        constexpr void pop_front ()
        {
            const auto lock = std::scoped_lock(mutex);
            container.pop_front();
        }

        template <typename... Args> 
        constexpr reference emplace_front (Args &&...args)
        {
            const auto lock = std::scoped_lock(mutex);
            return container.emplace_front(args...);
        }

        // TODO: expand to functions with standard conforming signatures
        template <typename... Args>
        constexpr void splice (Args ...args)
        {
            const auto lock = std::scoped_lock(mutex);
            container.splice(args...);
        }

        constexpr size_type remove (const value_type &value)
        {
            const auto lock = std::scoped_lock(mutex);
            container.remove(value);
        }

        template <typename UnaryPredicate>
        constexpr size_type remove_if (UnaryPredicate p)
        {
            const auto lock = std::scoped_lock(mutex);
            container.remove_if(p);
        }

        void reverse ()
        {
            const auto lock = std::scoped_lock(mutex);
            container.reverse();
        }

        template <typename BinaryPredicate>
        size_type unique (BinaryPredicate p)
        {
            const auto lock = std::scoped_lock(mutex);
            container.unique(p);
        }
        
        template <class Compare>
        void sort (Compare comp)
        {
            const auto lock = std::scoped_lock(mutex);
            container.sort(comp);
        }

        void merge (Container<Data> &&other)
        {
            const auto lock = std::scoped_lock(mutex, other.mutex);
            container.merge(other);
        }

        template <class Compare>
        void merge (Container<Data> &&other, Compare comp)
        {
            const auto lock = std::scoped_lock(mutex, other.mutex);
            container.merge(other, comp);
        }
};

template <typename Data>
class SequenceContainer : public Container<Data>
{
    public:
        using Container<Data>::Container;

        using Container<Data>::rbegin;
        using Container<Data>::rend;
        using Container<Data>::crbegin;
        using Container<Data>::crend;
        using Container<Data>::resize;
        using Container<Data>::assign;
        using Container<Data>::push_back;
        using Container<Data>::pop_back;
        using Container<Data>::emplace_back;
};

template <typename T, typename Data = std::pmr::vector<T>>
class Vector : public SequenceContainer<Data>
{
    public:
        using SequenceContainer<Data>::SequenceContainer;
        USING_TYPES

        using Container<Data>::at;
        using Container<Data>::operator[];
        using Container<Data>::data;
        using Container<Data>::capacity;
        using Container<Data>::reserve;
        using Container<Data>::shrink_to_fit;
};

template <typename T, typename Data = std::pmr::deque<T>>
class Deque : public SequenceContainer<Data>
{
    public:
        using SequenceContainer<Data>::SequenceContainer;
        USING_TYPES

        using Container<Data>::at;
        using Container<Data>::operator[];
        using Container<Data>::shrink_to_fit;
        using Container<Data>::push_front;
        using Container<Data>::pop_front;
        using Container<Data>::emplace_front;
};

template <typename T, typename Data = std::pmr::list<T>>
class List : public SequenceContainer<Data>
{
    public:
        using SequenceContainer<Data>::SequenceContainer;
        USING_TYPES

        using Container<Data>::push_front;
        using Container<Data>::pop_front;
        using Container<Data>::emplace_front;
        using Container<Data>::splice;
        using Container<Data>::remove;
        using Container<Data>::remove_if;
        using Container<Data>::reverse;
        using Container<Data>::unique;
        using Container<Data>::sort;
};

template <typename Data>
class AssociativeContainer : public Container<Data>
{
    public:
        using Container<Data>::Container;

        using Container<Data>::rbegin;
        using Container<Data>::rend;
        using Container<Data>::crbegin;
        using Container<Data>::crend;
        using Container<Data>::merge;
};

template <typename Key, typename Data = std::pmr::set<Key>>
class Set
{
    public:
        using AssociativeContainer<Data>::AssociativeContainer;
        USING_TYPES

        
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
