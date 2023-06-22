#include <frt/frt.hpp>

using namespace FRT::unit_literals;

// write your own functions here

int main () 
{
    std::ios_base::sync_with_stdio(false);

    // write driver code here
    FRT::Logger::info("Hello World!");

    FRT::GyroSensor gyro(INPUT_1);
    while (true) {
        const auto res = gyro.get_angle_and_rate();
        FRT::Logger::info(res.angle, res.rate);
    }
}
