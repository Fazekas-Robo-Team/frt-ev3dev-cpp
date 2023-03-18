#pragma once

#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>
#include <type_traits>
#include <string>
#include <limits>

using namespace std::chrono_literals;

namespace FRT 
{

const long double pi = acos(-1);

template <typename T>
inline auto clamp (const T &value, const T &min, const T &max)
{
    return std::min(std::max(value, min), max);
}

/// @brief Shorthand for std::this_thread::sleep_for.
/// @param value Duration. Accepts std::chrono_literals literals, such as sleep(500ms).
template <typename T>
inline void sleep (const T &duration)
{
    std::this_thread::sleep_for(duration);
}

/// @returns Timestamp in seconds with the resolution of microseconds.
inline double time ()
{
    using namespace std::chrono;

    const auto now = system_clock::now();
    const auto ms = duration_cast<microseconds>(now.time_since_epoch());
    return (double)ms.count() / 1e6;
}

template <typename T>
class UnitBase
{
    public:
        const double value;

        static std::string postfix;

        UnitBase (const long double &value) : value(value) {}

        UnitBase () = delete;
        UnitBase (const UnitBase &) = default;
        UnitBase (UnitBase &&) = default;

        friend std::ostream &operator<< (std::ostream &stream, const UnitBase &value)
        {
            stream << value.value << " " << value.postfix;
            return stream;
        }

        constexpr T operator + (const T &rhs) const
        {
            return T(value + rhs.value);
        }

        constexpr T operator - (const T &rhs) const 
        {
            return T(value - rhs.value);
        }

        template <typename RHS>
        requires std::is_arithmetic_v<RHS>
        friend constexpr T operator * (const UnitBase<T> &lhs, const RHS &rhs)
        {
            return T(lhs.value * rhs);
        }

        template <typename RHS>
        requires std::is_arithmetic_v<RHS>
        friend constexpr T operator * (const RHS &rhs, const UnitBase<T> &lhs)
        {
            return T(lhs.value * rhs);
        }

        template <typename RHS>
        requires std::is_arithmetic_v<RHS>
        constexpr T operator / (const RHS &rhs) const
        {
            return T(value / rhs);
        }

        constexpr auto operator<=> (const T &rhs) const 
        {
            return value - rhs.value <=> 0; 
        }

        constexpr bool operator== (const T &rhs) const
        {
            return fabs(value - rhs.value) < 1e-6;
        }

        constexpr T operator- () const
        {
            return T(-value);
        }
};

template <typename T>
class LengthUnit;

template <class T> 
concept Length = std::is_base_of_v<LengthUnit<T>, T>;

// predeclaration for LengthUnit
template <Length To, Length From>
constexpr inline To length_cast (const From &length);

template <typename T>
class LengthUnit : public UnitBase<T>
{
    public:
        using UnitBase<T>::UnitBase;

        template <Length To>
        constexpr operator To () const
        {
            return length_cast<To>(T(this->value));
        }
};

template <typename T>
class AngleUnit;

template <class T> 
concept Angle = std::is_base_of_v<AngleUnit<T>, T>;

// predeclaration for class AngleUnit
template <Angle To, Angle From>
constexpr inline To angle_cast (const From &angle);

template <typename T>
class AngleUnit : public UnitBase<T>
{
    public:
        using UnitBase<T>::UnitBase;

        template <Angle To>
        constexpr operator To () const
        {
            return angle_cast<To>(T(this->value));
        }
};

template <typename T>
concept Unit = Length<T> || Angle<T>;

class mm : public LengthUnit<mm> { public: using LengthUnit<mm>::LengthUnit; };
class cm : public LengthUnit<cm> { public: using LengthUnit<cm>::LengthUnit; };
class dm : public LengthUnit<dm> { public: using LengthUnit<dm>::LengthUnit; };
class m : public LengthUnit<m> { public: using LengthUnit<m>::LengthUnit; };

class deg : public AngleUnit<deg> { public: using AngleUnit<deg>::AngleUnit; };
class rad : public AngleUnit<rad> { public: using AngleUnit<rad>::AngleUnit; };
class grad : public AngleUnit<grad> { public: using AngleUnit<grad>::AngleUnit; };

// deprecated, will be removed soon
using _mm = mm;
using _cm = cm;
using _dm = dm;
using _m = m;
using _deg = deg;
using _rad = rad;
using _grad = grad;

template <typename T> std::string UnitBase<T>::postfix("units");
template <> std::string UnitBase<mm>::postfix("mm");
template <> std::string UnitBase<cm>::postfix("cm");
template <> std::string UnitBase<dm>::postfix("dm");
template <> std::string UnitBase<m>::postfix("m");
template <> std::string UnitBase<deg>::postfix("deg");
template <> std::string UnitBase<rad>::postfix("rad");
template <> std::string UnitBase<grad>::postfix("grad");

namespace unit_literals 
{
#define _UNIT_LITERAL(X) X operator"" X (const long double value) { return X(value); } \
                        X operator"" X (const unsigned long long value) { return X(value); }
    _UNIT_LITERAL(mm)
    _UNIT_LITERAL(cm)
    _UNIT_LITERAL(dm)
    _UNIT_LITERAL(m)

    _UNIT_LITERAL(deg)
    _UNIT_LITERAL(rad)
    _UNIT_LITERAL(grad)
};

template <Angle To, Angle From>
constexpr inline To angle_cast (const From &angle) 
{
    if constexpr (std::is_same_v<From, To>) {
        return angle;
    }
    if constexpr (std::is_same_v<To, rad>) {
        // conversions from other units to radian
        if constexpr (std::is_same_v<From, deg>) return rad(angle.value / 180 * pi);
        if constexpr (std::is_same_v<From, grad>) return rad(angle.value / 200 * pi);
    }
    if constexpr (std::is_same_v<From, rad>) {
        // conversion from radian to other units
        if constexpr (std::is_same_v<To, deg>) return deg(angle.value * 180 / pi);
        if constexpr (std::is_same_v<To, grad>) return grad(angle.value * 200 / pi);
    }
    // from a unit to radian to another unit
    const auto radians = angle_cast<rad>(angle);
    return angle_cast<To>(radians);
}

template <Length To, Length From>
constexpr inline To length_cast (const From &length) 
{
    if constexpr (std::is_same_v<From, To>) {
        return length;
    }
    if constexpr (std::is_same_v<To, m>) {
        // conversions from other units to meter
        if constexpr (std::is_same_v<From, mm>) return m(length.value / 1000);
        if constexpr (std::is_same_v<From, cm>) return m(length.value / 100);
        if constexpr (std::is_same_v<From, dm>) return m(length.value / 10);
    }
    if constexpr (std::is_same_v<From, m>) {
        // conversion from meter to other units
        if constexpr (std::is_same_v<To, mm>) return mm(length.value * 1000);
        if constexpr (std::is_same_v<To, cm>) return cm(length.value * 100);
        if constexpr (std::is_same_v<To, dm>) return dm(length.value * 10);
    }
    // from a unit to meters to another unit
    const auto meters = length_cast<m>(length);
    return length_cast<To>(meters);
}

template <Length To>
constexpr inline To length_cast (const Angle auto &angle, const Length auto &diameter)
{
    const rad radians = angle_cast<rad>(angle);
    const m diameter_meters = length_cast<m>(diameter);
    const m arc = radians.value * diameter_meters.value / 2;
    return length_cast<To>(arc);
}

template <Angle To>
constexpr inline To angle_cast (const Length auto &arc_length, const Length auto &diameter)
{
    const m arc_meters = length_cast<m>(arc_length);
    const m diameter_meters = length_cast<m>(diameter);
    const rad radians = 2 * arc_meters.value / diameter_meters.value;
    return angle_cast<To>(radians);
}

template <Angle To>
constexpr inline To pulses_to_units (const double pulses, [[maybe_unused]] const Length auto &diameter, const int pulses_per_rotation)
{
    const auto radians = rad(2 * pi * pulses / pulses_per_rotation);
    return angle_cast<To>(radians);
}

template <Length To>
constexpr inline To pulses_to_units (const double pulses, const Length auto &diameter, const int pulses_per_rotation)
{
    const auto radians = pulses_to_units<rad>(pulses, diameter, pulses_per_rotation);
    return length_cast<To>(radians, diameter);
}

template <typename To>
requires std::is_arithmetic_v<To>
constexpr inline To pulses_to_units (const double pulses, [[maybe_unused]] const Length auto &diameter, [[maybe_unused]] const int pulses_per_rotation)
{
    return pulses;
}

constexpr inline double units_to_pulses (const Angle auto &value, [[maybe_unused]] const Length auto &diameter, const int pulses_per_rotation)
{
    const auto radians = angle_cast<rad>(value);
    return radians.value / pi / 2 * pulses_per_rotation;
}

constexpr inline double units_to_pulses (const Length auto &value, const Length auto &diameter, const int pulses_per_rotation)
{
    const auto radians = angle_cast<rad>(value, diameter);
    return units_to_pulses(radians, diameter, pulses_per_rotation);
}

constexpr inline double units_to_pulses (const double &value, [[maybe_unused]] const Length auto &diameter, [[maybe_unused]] const int pulses_per_rotation)
{
    return value;
}

}; // namespace
