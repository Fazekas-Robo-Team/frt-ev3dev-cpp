#pragma once

#include "device.hpp"

namespace FRT
{

class TachoMotorInterface : public Device
{
    public:
        using Device::Device;

        FRT::File address = attribute("address");
        FRT::File command = attribute("command");
        FRT::File commands = attribute("commands");
        FRT::File count_per_rot = attribute("count_per_rot");
        FRT::File count_per_m = attribute("count_per_m");
        FRT::File full_travel_count = attribute("full_travel_count");
        FRT::File driver_name = attribute("driver_name");
        FRT::File duty_cycle = attribute("duty_cycle");
        FRT::File duty_cycle_sp = attribute("duty_cycle_sp");
        FRT::File polarity = attribute("polarity");
        FRT::File position = attribute("position");
        FRT::File hold_pid_kd = attribute("hold_pid/Kd");
        FRT::File hold_pid_ki = attribute("hold_pid/Ki");
        FRT::File hold_pid_kp = attribute("hold_pid/Kp");
        FRT::File max_speed = attribute("max_speed");
        FRT::File position_sp = attribute("position_sp");
        FRT::File speed = attribute("speed");
        FRT::File speed_sp = attribute("speed_sp");
        FRT::File ramp_up_sp = attribute("ramp_up_sp");
        FRT::File ramp_down_sp = attribute("ramp_down_sp");
        FRT::File speed_pid_kd = attribute("speed_pid/Kd");
        FRT::File speed_pid_ki = attribute("speed_pid/Ki");
        FRT::File speed_pid_kp = attribute("speed_pid/Kp");
        FRT::File state = attribute("state");
        FRT::File stop_action = attribute("stop_action");
        FRT::File stop_actions = attribute("stop_actions");
        FRT::File time_sp = attribute("time_sp");
};

class TachoMotor
{
    public:
        struct modes {
            static constexpr std::string_view run_forever = "run-forever";
            static constexpr std::string_view run_to_absolute_position = "run-to-abs-pos";
            static constexpr std::string_view run_to_relative_position = "run-to-rel-pos";
            static constexpr std::string_view run_timed = "run-timed";
            static constexpr std::string_view run_direct = "run-direct";
            static constexpr std::string_view stop = "stop";
            static constexpr std::string_view reset = "reset";
        };

        struct polarities {
            static constexpr std::string_view normal = "normal";
            static constexpr std::string_view inversed = "inversed";
        };

        struct states {
            static constexpr std::string_view running = "running";
            static constexpr std::string_view ramping = "ramping";
            static constexpr std::string_view holding = "holding";
            static constexpr std::string_view overloaded = "overloaded";
            static constexpr std::string_view stalled = "stalled";
        };

        struct stop_actions {
            static constexpr std::string_view coast = "coast";
            static constexpr std::string_view brake = "brake";
            static constexpr std::string_view hold = "hold";
        };

    private:
        TachoMotorInterface attributes;

        // user-set attributes
        int ramp_up_setpoint = -1;
        int ramp_down_setpoint = -1;
        int speed_setpoint = 0;
        int position_setpoint = 0;
        int duty_cycle_setpoint = 0;
        std::string_view polarity = "";
        std::string_view stop_action = "";

    public:
        // constant attributes
        const _m diameter;
        const std::string port;
        const FRT::Set<std::string> supported_modes;
        const int pulses_per_rotation;
        const std::string driver_name;
        const int max_speed;
        const FRT::Set<std::string> supported_stop_actions;

        template <typename Unit>
        TachoMotor (const std::string &port, const Unit &diameter, const bool initialize = true) 
        :   attributes("tacho-motor/", port),
            diameter(length_cast<_m>(diameter)),
            port(attributes.address.read<std::string>()),
            supported_modes(attributes.commands.read<FRT::Set<std::string>>()),
            pulses_per_rotation(attributes.count_per_rot.read<int>()),
            driver_name(attributes.driver_name.read<std::string>()),
            max_speed(attributes.max_speed.read<int>()),
            supported_stop_actions(attributes.stop_actions.read<FRT::Set<std::string>>())
        {
            if (!initialize) return;

            set_position(0);
            set_polarity(TachoMotor::polarities::normal);
            set_stop_action(TachoMotor::stop_actions::coast);
        }

        TachoMotor (const TachoMotor &) = delete;

        struct {
            double position_coefficient = 1;
        } config;

        template <typename Unit>
        constexpr Unit pulses_to_units (const double pulses)
        {
            const auto value = FRT::pulses_to_units<Unit>(pulses, diameter, pulses_per_rotation);
            return value * config.position_coefficient;
        }

        template <typename Unit>
        constexpr int units_to_pulses (const Unit &value)
        {
            const auto pulses = FRT::units_to_pulses(value, diameter, pulses_per_rotation);
            return round(value / config.position_coefficient);
        }

        void set_mode (const std::string_view &value) 
        {
            attributes.command.write(value);
        }

        std::string_view get_stop_action ()
        {
            return stop_action;
        }

        void set_stop_action (const std::string_view &value) 
        {
            if (value != stop_action) {
                stop_action = value;
                attributes.stop_action.write(value);
            }
        }

        int get_duty_cycle () 
        {
            return attributes.duty_cycle.read<int>();
        }

        int get_duty_cycle_setpoint ()
        {
            return duty_cycle_setpoint;
        }

        void set_duty_cycle_setpoint (const int value)
        {
            const auto percent = clamp(value, -100, 100);
            if (percent != duty_cycle_setpoint) {
                duty_cycle_setpoint = percent;
                attributes.duty_cycle_sp.write(percent);
            }
        }

        std::string_view get_polarity () 
        {
            return polarity;
        }

        void set_polarity (const std::string_view &value) 
        {
            if (value != polarity) {
                polarity = value;
                attributes.polarity.write(value);
            }
        }

        FRT::Vector<int> get_hold_pid ()
        {
            const int kp = attributes.hold_pid_kp.read<int>();
            const int ki = attributes.hold_pid_ki.read<int>();
            const int kd = attributes.hold_pid_kd.read<int>();
            return { kp, ki, kd };
        }

        void set_hold_pid (const int kp, const int ki, const int kd)
        {
            attributes.hold_pid_kp.write(kp);
            attributes.hold_pid_ki.write(ki);
            attributes.hold_pid_kd.write(kd);
        }

        FRT::Vector<int> get_speed_pid ()
        {
            const int kp = attributes.speed_pid_kp.read<int>();
            const int ki = attributes.speed_pid_ki.read<int>();
            const int kd = attributes.speed_pid_kd.read<int>();
            return { kp, ki, kd };
        }

        void set_speed_pid (const int kp, const int ki, const int kd)
        {
            attributes.speed_pid_kp.write(kp);
            attributes.speed_pid_ki.write(ki);
            attributes.speed_pid_kd.write(kd);
        }

        template <typename Unit>
        Unit get_position_setpoint ()
        {
            return TachoMotor::pulses_to_units<Unit>(position_setpoint);
        }

        template <typename Unit>
        void set_position_setpoint (const Unit &value)
        {
            const auto pulses = TachoMotor::units_to_pulses(value);
            if (pulses != position_setpoint) {
                position_setpoint = pulses;
                attributes.position_sp.write(pulses);
            }
        }

        template <typename Unit>
        Unit get_position ()
        {
            const auto pulses = attributes.position.read<int>();
            return TachoMotor::pulses_to_units<Unit>(pulses);
        }

        template <typename Unit>
        void set_position (const Unit &value)
        {
            const auto pulses = TachoMotor::units_to_pulses(value);
            attributes.position.write(pulses);
        }

        template <typename Unit>
        Unit get_speed ()
        {
            const auto pulses = attributes.speed.read<int>();
            return TachoMotor::pulses_to_units<Unit>(pulses);
        }

        template <typename Unit>
        Unit get_speed_setpoint ()
        {
            return TachoMotor::pulses_to_units<Unit>(speed_setpoint);
        }

        template <typename Unit>
        void set_speed_setpoint (const Unit &value)
        {
            auto pulses = TachoMotor::units_to_pulses(value);
            pulses = clamp(pulses, -max_speed, max_speed);
            if (pulses != speed_setpoint) {
                speed_setpoint = pulses;
                attributes.speed_sp.write(pulses);
            }
        }

        /// @return Milliseconds.
        int get_ramp_up_setpoint ()
        {
            return ramp_up_setpoint;
        }

        void set_ramp_up_setpoint (const int milliseconds)
        {
            if (milliseconds != ramp_up_setpoint) {
                ramp_up_setpoint = milliseconds;
                attributes.ramp_up_sp.write(milliseconds);
            }
        }

        /// @return Milliseconds.
        int get_ramp_down_setpoint ()
        {
            return ramp_down_setpoint;
        }

        void set_ramp_down_setpoint (const int milliseconds)
        {
            if (milliseconds != ramp_down_setpoint) {
                ramp_down_setpoint = milliseconds;
                attributes.ramp_down_sp.write(milliseconds);
            }
        }

        Set<std::string> get_state ()
        {
            return attributes.state.read<Set<std::string>>();
        }

        void wait_until (const std::string_view &state)
        {
            while (!get_state().count((std::string)state)) {
                attributes.state.wait();
            }
        }

        void wait_while (const std::string_view &state)
        {
            while (get_state().count((std::string)state)) {
                attributes.state.wait();
            }
        }

        /// @brief Rotate the motor forever.
        /// @param velocity Distance or rotation per second.
        /// @param brake Defaults to true.
        /// @param block Defaults to true.
        template <typename Unit, bool block = false>
        void on (const Unit &velocity)
        {
            set_speed_setpoint(velocity);
            set_mode(TachoMotor::modes::run_forever);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }

        template <bool block = true, bool brake = true, typename DistanceUnit, typename VelocityUnit>
        void on_for_segment (const DistanceUnit &segment, const VelocityUnit &velocity)
        {
            set_position_setpoint(segment);
            set_speed_setpoint(velocity);

            if constexpr (brake) {
                set_stop_action(TachoMotor::stop_actions::brake);
            }
            else {
                set_stop_action(TachoMotor::stop_actions::coast);
            }

            set_mode(TachoMotor::modes::run_to_relative_position);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }

        template <typename DistanceUnit, typename VelocityUnit, bool block = true, bool brake = true>
        void on_to_position (const DistanceUnit &position, const VelocityUnit &velocity)
        {
            set_position_setpoint(position);
            set_speed_setpoint(velocity);

            if constexpr (brake) {
                set_stop_action(TachoMotor::stop_actions::brake);
            }
            else {
                set_stop_action(TachoMotor::stop_actions::coast);
            }

            set_mode(TachoMotor::modes::run_to_absolute_position);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }
};

}; // namespace
