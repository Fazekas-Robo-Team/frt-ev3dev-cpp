#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"

#include <iostream>
#include <chrono>

using namespace std::chrono;

int main () 
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cerr.tie(0);

    auto start = high_resolution_clock::now();

    FRT::Vector<std::string> vector;
    FRT::Deque<std::string> deque;
    FRT::List<std::string> list;
    FRT::Set<std::string> set;
    FRT::Map<std::string, std::string> map;
    FRT::UnorderedSet<std::string> uset;
    FRT::UnorderedMap<std::string, std::string> umap;
    

    const auto stop = high_resolution_clock::now();
    const auto duration = duration_cast<nanoseconds>(stop - start);

    FRT::Logger::info("time elapsed:", (float)duration.count() / 1e9);
}
