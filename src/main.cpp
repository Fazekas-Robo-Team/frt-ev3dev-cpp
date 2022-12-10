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

    file.write(69);
    int i = file.read<int>();

    file.write(69);
    int j = file.read<int>();

    FRT::Logger::info(i, j);
}
