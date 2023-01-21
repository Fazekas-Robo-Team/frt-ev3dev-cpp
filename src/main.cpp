#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

// diameter is written onto the side of the wheels
TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_B, 6.2mm);

// measure it with tape, then refine during testing
const auto axle_length = 14.9cm; 

template <class DistanceUnit, class SpeedUnit>
void tank_move (const DistanceUnit distance, const SpeedUnit speed)
{
    const auto left_target = left.get_position() + distance;
    while (left_target >= left.get_position()) {
        left.on(speed);
        right.on(speed);
    }
    left.stop();
    right.stop();
}

template <class AngleUnit, class SpeedUnit>
void tank_turn (const AngleUnit angle, const SpeedUnit speed)
{
    const auto left_arc = angle * (axle_length / left.diameter);
    const auto left_target = left.get_position<_deg>() + left_arc;
    while (left_target >= left.get_position<_deg>()) {
        left.on(speed);
        right.on(-speed);
    }
    left.stop();
    right.stop();
}

int main () 
{
    std::ios_base::sync_with_stdio(false);

    while (true) {
        tank_move(50.0cm, 500.0deg /* per second */);
        tank_turn(90.0deg, 400.0deg /* per second */);
    }
}
