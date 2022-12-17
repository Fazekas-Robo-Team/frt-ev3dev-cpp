#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"
#include "file.hpp"
#include "device.hpp"
#include "utility.hpp"

#include <iostream>
#include <chrono>
#include <thread>

using namespace FRT;

int main () 
{
    std::ios_base::sync_with_stdio(false);
    
    auto left = TachoMotor(OUTPUT_A, 62.0mm);
    auto right = TachoMotor(OUTPUT_B, 62.0mm);

    left.set_polarity( TachoMotor::polarities::inversed );

    /*left.set_position_setpoint(1.5m);
    right.set_position_setpoint(1.5m);

    left.set_speed_setpoint(800.0deg);
    right.set_speed_setpoint(800.0deg);

    left.set_mode( left.modes.run_to_relative_position );
    right.set_mode( right.modes.run_to_relative_position );

    sleep(100ms);*/

    while (left.get_state() != TachoMotor::states::holding) {
        Logger::info( left.get_position<_cm>(), right.get_position<_cm>() );
    }
}
