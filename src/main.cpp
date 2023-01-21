#include <frt/frt.hpp>

// enables the use of unit suffixes, for example "6.0cm"
using namespace FRT::unit_literals;

FRT::TachoMotor left(OUTPUT_A, 6.0cm), right(OUTPUT_B, 6.0cm);

int main () 
{
    // speeds up input-output a bit
    std::ios_base::sync_with_stdio(false);

    // write driver code here
    FRT::Logger::info("Hello World!");
}
