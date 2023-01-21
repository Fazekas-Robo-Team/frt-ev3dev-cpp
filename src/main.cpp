#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_B, 6.2mm);

int main () 
{
    left.on(500.0deg);
    right.on<true>(500.0deg);
}
