#pragma once

#include "config.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <type_traits>

namespace FRT
{

template <typename A, typename B> 
std::ostream &operator<< (std::ostream &os, const std::pair<A, B> &p) 
{ 
    return os << '(' << p.first << ", " << p.second << ')'; 
}

template <class ContainerType> 
concept Container = requires(ContainerType a, const ContainerType b) 
{
    requires std::regular<ContainerType>;
    requires std::swappable<ContainerType>;
    requires std::destructible<typename ContainerType::value_type>;
    requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
    requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
    requires std::forward_iterator<typename ContainerType::iterator>;
    requires std::forward_iterator<typename ContainerType::const_iterator>;
    requires std::signed_integral<typename ContainerType::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename ContainerType::iterator>;
    { a.end() } -> std::same_as<typename ContainerType::iterator>;
    { b.begin() } -> std::same_as<typename ContainerType::const_iterator>;
    { b.end() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cbegin() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cend() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.size() } -> std::same_as<typename ContainerType::size_type>;
    { a.max_size() } -> std::same_as<typename ContainerType::size_type>;
    { a.empty() } -> std::convertible_to<bool>;
    requires !std::convertible_to<ContainerType, const std::string>;
    requires !std::convertible_to<ContainerType, const std::string_view>;
};

template <Container C>
std::ostream &operator<< (std::ostream &stream, const C &container) 
{ 
    stream << "{ "; 
    std::string separator; 
    for (const auto &x : container) {
        stream << separator << x; 
        separator = ", ";
    }
    return stream << " }"; 
}

class Logger 
{
    public:
        template <typename... Args>
        static inline void error (Args... args) 
        {
            if constexpr (log_level >= log_levels::error) {
                std::cout << "\u001b[31;1m[";
                print_time();
                std::cout << " ERROR]  ";
                print(args...);
            }
        }

        template <typename... Args>
        static inline void warning (Args... args) 
        {
            if constexpr (log_level >= log_levels::warning) {
                std::cout << "\u001b[33;1m[";
                print_time();
                std::cout << " WARNING]";
                print(args...);
            }
        }

        template <typename... Args>
        static inline void info (Args... args) 
        {
            if constexpr (log_level >= log_levels::info) {
                std::cout << "\u001b[0m[";
                print_time();
                std::cout << " INFO]   ";
                print(args...);
            }
        }

        template <typename... Args>
        static inline void debug (Args... args) 
        {
            if constexpr (log_level >= log_levels::debug) {
                std::cout << "\u001b[0m[";
                print_time();
                std::cout << " DEBUG]  ";
                print(args...);
            }
        }

    private:
        static inline void print_time ()
        {
            using namespace std::chrono;
            const auto now = system_clock::now();

            const auto timer = system_clock::to_time_t(now);
            const auto bt = std::localtime(&timer);
            std::cout << std::put_time(bt, "%H:%M:%S.");

            const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::cout << std::setfill('0') << std::setw(3) << ms.count();
        }

        template <typename Head, typename... Tail>
        static inline void print (Head head, Tail... tail)
        {
            std::cout << " " << head;
            print(tail...);
        }

        static inline void print ()
        {
            std::cout << std::endl;
        }
};

} // namespace