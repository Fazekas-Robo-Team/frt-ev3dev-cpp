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

using namespace FRT;

int clamp (const int value, const int min, const int max)
{
    return std::min(std::max(value, min), max);
}

int main () 
{
    std::ios_base::sync_with_stdio(false);

    /*Sensor color = Sensor(INPUT_1);
    color.attributes.mode.write("RGB-RAW");
    for (;;) {
        int r = color.attributes.value[0].read<int>();
        int g = color.attributes.value[1].read<int>();
        int b = color.attributes.value[2].read<int>();
        Logger::info(r, g, b);
    }*/

    auto left = TachoMotor(OUTPUT_A, /* diameter */ 5.6cm);
    auto right = TachoMotor(OUTPUT_B, /* diameter */ 5.6cm);
    
    left.set_position(0);
    right.set_position(0);

    left.config.position_coefficient = 1.0075;
    
    double base = 0;
    while (true) {
        double lp = left.get_position(), rp = right.get_position();
        if (base < 80) base += 0.5;
        double error = (lp - rp) / 80 * base;

        int ls = clamp(round(base - error), -100, 100), rs = clamp(round(base + error), -100, 100);

        left.set_duty_cycle_setpoint(ls);
        right.set_duty_cycle_setpoint(rs);

        left.set_mode( "run-direct" );
        right.set_mode( "run-direct" );

        FRT::Logger::info( lp, rp, ls, rs );
    }
}
