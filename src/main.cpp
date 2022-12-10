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

    std::pmr::vector<std::string> pmr;
    
    pmr.resize(3);

    for (auto &i : pmr) {
        std::cin >> i;
    }

    FRT::Vector<std::string> frt(std::move(pmr));
    
    FRT::Logger::info(frt);

    const auto stop = high_resolution_clock::now();
    const auto duration = duration_cast<nanoseconds>(stop - start);

    FRT::Logger::info("time elapsed:", (float)duration.count() / 1e9);
}
