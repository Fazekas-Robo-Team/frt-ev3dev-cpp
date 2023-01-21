#pragma once

#include "motor.hpp"

namespace FRT
{

class Tank
{
    public:
        TachoMotor &left, &right;

        template <typename LeftUnit, typename RightUnit, bool block = false>
        void on (const LeftUnit &left_velocity, const RightUnit &right_velocity)
        {
            left_motor.on<block>(left_velocity);
            right_motor.on<block>(right_velocity);
        }

        template <typename DistanceUnit, typename LeftVelocityUnit, typename RightVelocityUnit, bool block = true, bool brake = true>
        void on_for_segment (const DistanceUnit &segment, const LeftVelocityUnit &left_velocity, const RightVelocityUnit &right_velocity)
        {
            left_motor.on_for_segment<block, brake>(segment, left_velocity);
            left_motor.on_for_segment<block, brake>(segment, right_velocity);
        }

        template <typename DistanceUnit, typename LeftVelocityUnit, typename RightVelocityUnit, bool block = true, bool brake = true>
        void on_to_position (const DistanceUnit &position, const LeftVelocityUnit &left_velocity, const RightVelocityUnit &right_velocity)
        {
            left_motor.on_to_position<block, brake>(position, left_velocity);
            left_motor.on_to_position<block, brake>(position, right_velocity);
        }

        Tank (TachoMotor &left, TachoMotor &right)
        :   left(left), right(right) 
        {
            std::ios_base::sync_with_stdio(false);
        }

        Tank (const Tank &) = delete;

        virtual ~Tank () {}
};

}; // namespace
