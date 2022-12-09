#pragma once

#include "config.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>

namespace FRT
{

template <typename A, typename B> 
std::ostream &operator<< (std::ostream &os, const std::pair<A, B> &p) 
{ 
    return os << '(' << p.first << ", " << p.second << ')'; 
}

class Logger 
{
    public:
        template <typename... Args>
        static inline void error (Args... args) 
        {
            #if LOG_LEVEL >= 1
            std::cout << "\u001b[31;1m[";
            print_time();
            std::cout << " ERROR]  ";
            print(args...);
            #endif
        }

        template <typename... Args>
        static inline void warning (Args... args) 
        {
            #if LOG_LEVEL >= 2
            std::cout << "\u001b[33;1m[";
            print_time();
            std::cout << " WARNING]";
            print(args...);
            #endif
        }

        template <typename... Args>
        static inline void info (Args... args) 
        {
            #if LOG_LEVEL >= 3
            std::cout << "\u001b[0m[";
            print_time();
            std::cout << " INFO]   ";
            print(args...);
            #endif
        }

        template <typename... Args>
        static inline void debug (Args... args) 
        {
            #if LOG_LEVEL >= 4
            std::cout << "\u001b[0m[";
            print_time();
            std::cout << " DEBUG]  ";
            print(args...);
            #endif
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