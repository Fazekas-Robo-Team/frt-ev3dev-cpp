#pragma once

#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>

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
        double value;

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

        operator double () const
        {
            return UnitBase<T>::value;
        }
};

// predeclaration for LengthUnit
template <typename To, typename From>
constexpr inline To length_cast (const From &length);

template <typename T>
class LengthUnit : public UnitBase<T>
{
    public:
        using UnitBase<T>::UnitBase;

        #define LENGTH_OPERATOR(X) \
            template <typename RHS> \
            friend constexpr T operator X (const LengthUnit<T> &lhs, const RHS &rhs) \
            { \
                if constexpr (std::is_arithmetic_v<RHS>) return T(lhs.value X rhs); \
                return T(lhs.value X length_cast<T>(rhs).value); \
            } \
            /*template <typename LHS> \
            friend constexpr T operator X (const LHS &lhs, const LengthUnit<T> &rhs) \
            { \
                if constexpr (std::is_arithmetic_v<LHS>) return T(lhs X rhs.value); \
                return T(length_cast<T>(lhs).value X rhs.value); \
            }*/

        LENGTH_OPERATOR(+);
        LENGTH_OPERATOR(-);
        LENGTH_OPERATOR(*);

        template <typename RHS>
        friend constexpr auto operator / (const LengthUnit<T> &lhs, const RHS &rhs)
        {
            if constexpr (std::is_arithmetic_v<RHS>) return T(lhs.value / rhs);
            return lhs.value / length_cast<T>(rhs).value;
        }

        template <typename RHS> 
        friend constexpr auto operator<=> (const LengthUnit<T> &lhs, const RHS &rhs)
        { 
            return lhs.value <=> length_cast<T>(rhs).value; 
        }

        template <typename RHS> 
        friend constexpr bool operator== (const LengthUnit<T> &lhs, const RHS &rhs)
        { 
            return lhs.value == length_cast<T>(rhs).value; 
        }

        constexpr T operator- () const
        {
            return T(-UnitBase<T>::value);
        }
};

// predeclaration for class AngleUnit
template <typename To, typename From>
constexpr inline To angle_cast (const From &angle);

template <typename T>
class AngleUnit : public UnitBase<T>
{
    public:
        using UnitBase<T>::UnitBase;

        #define ANGLE_OPERATOR(X) \
            template <typename RHS> \
            friend constexpr T operator X (const AngleUnit<T> &lhs, const RHS &rhs) \
            { \
                if constexpr (std::is_arithmetic_v<RHS>) return T(lhs.value X rhs); \
                return T(lhs.value X angle_cast<T>(rhs).value); \
            } \

        ANGLE_OPERATOR(+);
        ANGLE_OPERATOR(-);
        ANGLE_OPERATOR(*);

        template <typename RHS>
        friend constexpr T operator / (const AngleUnit<T> &lhs, const RHS &rhs)
        {
            if constexpr (std::is_arithmetic_v<RHS>) return T(lhs.value / rhs);
            return lhs.value / angle_cast<T>(rhs).value;
        }

        template <typename RHS> 
        friend constexpr auto operator<=> (const AngleUnit<T> &lhs, const RHS &rhs) 
        { 
            return lhs.value <=> angle_cast<T>(rhs).value; 
        }

        template <typename RHS> 
        friend constexpr bool operator== (const AngleUnit<T> &lhs, const RHS &rhs) 
        { 
            return lhs.value == angle_cast<T>(rhs).value; 
        }

        constexpr T operator- () const
        {
            return T(-UnitBase<T>::value);
        }
};

class _mm : public LengthUnit<_mm> { public: using LengthUnit<_mm>::LengthUnit; };
class _cm : public LengthUnit<_cm> { public: using LengthUnit<_cm>::LengthUnit; };
class _dm : public LengthUnit<_dm> { public: using LengthUnit<_dm>::LengthUnit; };
class _m : public LengthUnit<_m> { public: using LengthUnit<_m>::LengthUnit; };

class _deg : public AngleUnit<_deg> { public: using AngleUnit<_deg>::AngleUnit; };
class _rad : public AngleUnit<_rad> { public: using AngleUnit<_rad>::AngleUnit; };
class _grad : public AngleUnit<_grad> { public: using AngleUnit<_grad>::AngleUnit; };

template <typename T> std::string UnitBase<T>::postfix("units");
template <> std::string UnitBase<_mm>::postfix("mm");
template <> std::string UnitBase<_cm>::postfix("cm");
template <> std::string UnitBase<_dm>::postfix("dm");
template <> std::string UnitBase<_m>::postfix("m");
template <> std::string UnitBase<_deg>::postfix("deg");
template <> std::string UnitBase<_rad>::postfix("rad");
template <> std::string UnitBase<_grad>::postfix("grad");

namespace unit_literals 
{
    _mm operator""mm (const long double value) { return _mm(value); }
    _cm operator""cm (const long double value) { return _cm(value); }
    _dm operator""dm (const long double value) { return _dm(value); }
    _m operator""m (const long double value) { return _m(value); }

    _deg operator""deg (const long double value) { return _deg(value); }
    _rad operator""rad (const long double value) { return _rad(value); }
    _grad operator""grad (const long double value) { return _grad(value); }
};

template <typename To, typename From>
constexpr inline To angle_cast (const From &angle) 
{
    if constexpr (std::is_same_v<From, To>) {
        return angle;
    }
    if constexpr (std::is_same_v<To, _rad>) {
        // conversions from other units to radian
        if constexpr (std::is_same_v<From, _deg>) return _rad(angle.value / 180 * pi);
        if constexpr (std::is_same_v<From, _grad>) return _rad(angle.value / 200 * pi);
    }
    if constexpr (std::is_same_v<From, _rad>) {
        // conversion from radian to other units
        if constexpr (std::is_same_v<To, _deg>) return _deg(angle.value * 180 / pi);
        if constexpr (std::is_same_v<To, _grad>) return _grad(angle.value * 200 / pi);
    }
    // from a unit to radian to another unit
    const auto radians = angle_cast<_rad>(angle);
    return angle_cast<To>(radians);
}

template <typename To, typename From>
constexpr inline To length_cast (const From &length) 
{
    if constexpr (std::is_same_v<From, To>) {
        return length;
    }
    if constexpr (std::is_same_v<To, _m>) {
        // conversions from other units to meter
        if constexpr (std::is_same_v<From, _mm>) return _m(length.value / 1000);
        if constexpr (std::is_same_v<From, _cm>) return _m(length.value / 100);
        if constexpr (std::is_same_v<From, _dm>) return _m(length.value / 10);
    }
    if constexpr (std::is_same_v<From, _m>) {
        // conversion from meter to other units
        if constexpr (std::is_same_v<To, _mm>) return _mm(length.value * 1000);
        if constexpr (std::is_same_v<To, _cm>) return _cm(length.value * 100);
        if constexpr (std::is_same_v<To, _dm>) return _dm(length.value * 10);
    }
    // from a unit to meters to another unit
    const auto meters = length_cast<_m>(length);
    return length_cast<To>(meters);
}

template <typename To, typename Angle, typename From>
constexpr inline To length_cast (const Angle &angle, const From &diameter)
{
    const _rad radians = angle_cast<_rad>(angle);
    const _m diameter_meters = length_cast<_m>(diameter);
    const _m arc = radians.value * diameter_meters.value / 2;
    return length_cast<To>(arc);
}

template <typename To, typename From, typename Diameter>
constexpr inline To angle_cast (const From &arc_length, const Diameter &diameter)
{
    const _m arc_meters = length_cast<_m>(arc_length);
    const _m diameter_meters = length_cast<_m>(diameter);
    const _rad radians = 2 * arc_meters.value / diameter_meters.value;
    return angle_cast<To>(radians);
}

template <typename To, typename Diameter>
constexpr inline To pulses_to_units (const double pulses, const Diameter &diameter, const int pulses_per_rotation)
{
    if constexpr (std::is_base_of_v<AngleUnit<To>, To>) {
        const auto radians = _rad(2 * pi * pulses / pulses_per_rotation);
        return angle_cast<To>(radians);
    }
    if constexpr (std::is_base_of_v<LengthUnit<To>, To>) {
        const auto radians = pulses_to_units<_rad>(pulses, diameter, pulses_per_rotation);
        return length_cast<To>(radians, diameter);
    }
    if constexpr (std::is_arithmetic_v<To>) {
        return pulses;
    }
}

template <typename From, typename Diameter>
constexpr inline double units_to_pulses (const From &value, const Diameter &diameter, const int pulses_per_rotation)
{
    if constexpr (std::is_base_of_v<AngleUnit<From>, From>) {
        const auto radians = angle_cast<_rad>(value);
        return radians.value / pi / 2 * pulses_per_rotation;
    }
    if constexpr (std::is_base_of_v<LengthUnit<From>, From>)
    {
        const auto radians = angle_cast<_rad>(value, diameter);
        return units_to_pulses(radians, diameter, pulses_per_rotation);
    }
    if constexpr (std::is_arithmetic_v<From>) {
        return value;
    }
}

}