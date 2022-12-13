#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"
#include "file.hpp"
#include "device.hpp"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

int main () 
{
    std::ios_base::sync_with_stdio(false);

    FRT::Sensor color = FRT::Sensor(INPUT_1);
    color.attribute.mode.write("RGB-RAW");
    for (;;) {
        int r = color.attribute.value[0].read<int>();
        int g = color.attribute.value[1].read<int>();
        int b = color.attribute.value[2].read<int>();
        FRT::Logger::info(r, g, b);
    }

}
