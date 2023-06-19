#pragma once
#include <../include/frt/frt.hpp>
//#include "coor.hpp"
//#include "egyeb_old.hpp"
//#include "mod_old.hpp"


using namespace FRT;
using namespace FRT::unit_literals;

double fsztav=14.5;
int mva[2]={0};

TachoMotor left(OUTPUT_B, 6.2cm), right(OUTPUT_A, 6.2cm);
ColorSensor ceft(INPUT_1), cight(INPUT_2);
HTColorSensorV2 hol(INPUT_4);
TachoMotor emel(OUTPUT_D,6.2cm),kamp(OUTPUT_C,6.2cm);