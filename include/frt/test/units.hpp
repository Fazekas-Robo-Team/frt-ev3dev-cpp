#pragma once

#include <frt/src/utility.hpp>
#include <frt/src/logger.hpp>

#include <cassert>
#include <climits>

namespace FRT 
{

void units_test ()
{
    using namespace FRT::unit_literals;

    // assignments
    {
        m x = 1, y = 100cm;
        auto z = 1m, w = m(1);
        assert(x == y && x == z && x == w);
    }
    {
        grad x = 10, y = 9deg;
        auto z = 10grad, w = grad(10);
        assert(x == y && x == z && x == w);
    }

    // explicit conversions
    assert(length_cast<dm>(1.2m) == length_cast<dm>(120cm));
    assert(length_cast<cm>(12dm) == length_cast<cm>(1200mm));
    assert(9deg == angle_cast<deg>(10grad));

    // implicit conversions
    assert(1m == 10dm);
    assert(-10mm == -1cm);
    assert(10grad == 9deg);

    // conversion on perimeter
    assert(length_cast<dm>(3rad, 20cm) == 3dm);
    assert(length_cast<dm>(-3rad, 20cm) == -3dm);
    assert(angle_cast<rad>(3dm, 20cm) == 3rad);
    assert(angle_cast<rad>(-3dm, 20cm) == -3rad);

    // conversion to and from motor pulses
    assert(units_to_pulses(dm(pi / 2), 20cm, 10) == 2.5);
    assert(units_to_pulses(90deg, 20cm, 10) == 2.5);
    assert(units_to_pulses(2.5, 20cm, 10) == 2.5);
    assert(pulses_to_units<dm>(-2.5, 20cm, 10) == dm(-pi / 2));
    assert(pulses_to_units<deg>(2.5, 20cm, 10) == 90deg);
    assert(pulses_to_units<double>(2.5, 20cm, 10) == 2.5);

    // arithmetic
    assert(-50cm + 2m == 15dm);
    assert(15dm == 2m - 50cm);

    assert(2 * 9deg == 20grad);
    assert(9deg * 2 == 20grad); // commutativity

    assert(1.0cm / -2 == -5mm);
    
    // comparisons
    assert(5mm < 1cm);
    assert(3.14rad > 90deg);
    assert(1.0cm != 0.999999cm);
    assert(1.0cm == 0.9999999cm);
    
    // logging
    Logger::info(1m, 1dm, 1cm, 1mm, 1deg, 1rad, 1grad);
}

}; // namespace
