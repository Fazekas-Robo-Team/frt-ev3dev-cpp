#pragma once

#include "config.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <mutex>

namespace FRT
{

template <typename A, typename B> 
std::ostream &operator<< (std::ostream &os, const std::pair<A, B> &p) 
{ 
    return os << '(' << p.first << ", " << p.second << ')'; 
}

class Logger 
{
    private:
        static std::mutex mutex;

    public:
        template <typename... Args>
        static inline void error (Args... args) 
        {
            if constexpr (log_level >= log_levels::error) {
                const auto lock = std::scoped_lock(mutex);
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
                const auto lock = std::scoped_lock(mutex);
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
                const auto lock = std::scoped_lock(mutex);
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
                const auto lock = std::scoped_lock(mutex);
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

std::mutex Logger::mutex = std::mutex();

} // namespace