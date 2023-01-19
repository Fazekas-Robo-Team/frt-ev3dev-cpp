#include "logger.hpp"
#include "utility.hpp"
#include "motor.hpp"
#include "sensor.hpp"
#include "tank.hpp"
#include "scheduler.hpp"

using namespace FRT;

int main () 
{
    auto left = TachoMotor(OUTPUT_A, 6.2mm);
    auto right = TachoMotor(OUTPUT_B, 6.2mm);
    auto tank = Tank(left, right);

    tank.left.set_polarity(TachoMotor::polarities::inversed);
    tank.left.config.position_coefficient = 1.0075;

    tank.on_for_segment(150.0cm, 1020.0deg, 1020.0deg);
}

