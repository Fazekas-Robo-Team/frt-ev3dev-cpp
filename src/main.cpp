#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"
#include "file.hpp"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

int main () 
{
    std::ios_base::sync_with_stdio(false);

    struct {
        FRT::File driver_name = FRT::File("/sys/class/lego-sensor/sensor0/driver_name");
        FRT::File modes = FRT::File("/sys/class/lego-sensor/sensor0/modes");
        FRT::File mode = FRT::File("/sys/class/lego-sensor/sensor0/mode");
        FRT::File value = FRT::File("/sys/class/lego-sensor/sensor0/value0");
    } color_sensor;
    int i = 0;
    for (;;) {
        for (int i = 0; i < 999; i++) color_sensor.value.read<int>();
        FRT::Logger::info( i );
        i++;
    }
}
