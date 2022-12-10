#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"
#include "file.hpp"

#include <iostream>
#include <chrono>

using namespace std::chrono;

int main () 
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cerr.tie(0);

    auto file = FRT::File("asd");

    const auto set = file.read<FRT::Set<std::string>>();

    FRT::Logger::info(set);
}
