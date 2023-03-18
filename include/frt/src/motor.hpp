#pragma once

#include "device.hpp"

namespace FRT
{

class TachoMotorInterface : public Device
{
    public:
        using Device::Device;

        File address = attribute("address");
        File command = attribute("command");
        File commands = attribute("commands");
        File count_per_rot = attribute("count_per_rot");
        File count_per_m = attribute("count_per_m");
        File full_travel_count = attribute("full_travel_count");
        File driver_name = attribute("driver_name");
        File duty_cycle = attribute("duty_cycle");
        File duty_cycle_sp = attribute("duty_cycle_sp");
        File polarity = attribute("polarity");
        File position = attribute("position");
        File hold_pid_kd = attribute("hold_pid/Kd");
        File hold_pid_ki = attribute("hold_pid/Ki");
        File hold_pid_kp = attribute("hold_pid/Kp");
        File max_speed = attribute("max_speed");
        File position_sp = attribute("position_sp");
        File speed = attribute("speed");
        File speed_sp = attribute("speed_sp");
        File ramp_up_sp = attribute("ramp_up_sp");
        File ramp_down_sp = attribute("ramp_down_sp");
        File speed_pid_kd = attribute("speed_pid/Kd");
        File speed_pid_ki = attribute("speed_pid/Ki");
        File speed_pid_kp = attribute("speed_pid/Kp");
        File state = attribute("state");
        File stop_action = attribute("stop_action");
        File stop_actions = attribute("stop_actions");
        File time_sp = attribute("time_sp");
};

class TachoMotor
{
    public:
        struct commands 
        {
            static constexpr std::string_view run_forever = "run-forever";
            static constexpr std::string_view run_to_absolute_position = "run-to-abs-pos";
            static constexpr std::string_view run_to_relative_position = "run-to-rel-pos";
            static constexpr std::string_view run_timed = "run-timed";
            static constexpr std::string_view run_direct = "run-direct";
            static constexpr std::string_view stop = "stop";
            static constexpr std::string_view reset = "reset";
        };

        struct polarities 
        {
            static constexpr std::string_view normal = "normal";
            static constexpr std::string_view inversed = "inversed";
        };

        struct states 
        {
            static constexpr std::string_view running = "running";
            static constexpr std::string_view ramping = "ramping";
            static constexpr std::string_view holding = "holding";
            static constexpr std::string_view overloaded = "overloaded";
            static constexpr std::string_view stalled = "stalled";
        };

        struct stop_actions 
        {
            static constexpr std::string_view coast = "coast";
            static constexpr std::string_view brake = "brake";
            static constexpr std::string_view hold = "hold";
        };

    private:
        // user-set attributes
        int ramp_up_setpoint = -1;
        int ramp_down_setpoint = -1;
        int speed_setpoint = 0;
        int position_setpoint = 0;
        int duty_cycle_setpoint = 0;
        std::string_view polarity = "";
        std::string_view stop_action = "";

    public:
        TachoMotorInterface attributes;

        // constant attributes
        const m diameter;
        const std::string port;
        const std::vector<std::string> supported_commands;
        const int pulses_per_rotation;
        const std::string driver_name;
        const int max_speed;
        const std::vector<std::string> supported_stop_actions;

        TachoMotor (const std::string_view port, const Unit auto &diameter, const bool reset = true) 
        :   attributes("tacho-motor/", port),
            diameter(length_cast<m>(diameter)),
            port(attributes.address.read<std::string>()),
            supported_commands(attributes.commands.read<std::vector<std::string>>()),
            pulses_per_rotation(attributes.count_per_rot.read<int>()),
            driver_name(attributes.driver_name.read<std::string>()),
            max_speed(attributes.max_speed.read<int>()),
            supported_stop_actions(attributes.stop_actions.read<std::vector<std::string>>())
        {
            if (!reset) return;
            TachoMotor::reset();
        }

        struct 
        {
            double position_coefficient = 1;
        } config;

        template <Unit To>
        constexpr To pulses_to_units (const double pulses)
        {
            const auto value = FRT::pulses_to_units<To>(pulses, diameter, pulses_per_rotation);
            return value * config.position_coefficient;
        }

        constexpr int units_to_pulses (const Unit auto &value)
        {
            const auto pulses = FRT::units_to_pulses(value, diameter, pulses_per_rotation);
            return round(pulses / config.position_coefficient);
        }

        void run_command (const std::string_view command) 
        {
            attributes.command.write(command);
        }

        void run_forever ()
        {
            run_command(TachoMotor::commands::run_forever);
        }

        void run_to_absolute_position ()
        {
            run_command(TachoMotor::commands::run_to_absolute_position);
        }

        void run_to_relative_position ()
        {
            run_command(TachoMotor::commands::run_to_relative_position);
        }

        void run_timed ()
        {
            run_command(TachoMotor::commands::run_timed);
        }

        void run_direct ()
        {
            run_command(TachoMotor::commands::run_direct);
        }

        void stop ()
        {
            run_command(TachoMotor::commands::stop);
        }

        void reset ()
        {
            run_command(TachoMotor::commands::reset);
        }

        std::string_view get_stop_action ()
        {
            return stop_action;
        }

        void set_stop_action (const std::string_view value) 
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

        void set_polarity (const std::string_view value) 
        {
            if (value != polarity) {
                polarity = value;
                attributes.polarity.write(value);
            }
        }

        std::vector<int> get_hold_pid ()
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

        std::vector<int> get_speed_pid ()
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

        template <Unit Unit = cm>
        Unit get_position_setpoint ()
        {
            return TachoMotor::pulses_to_units<Unit>(position_setpoint);
        }

        void set_position_setpoint (const Unit auto &value)
        {
            const auto pulses = TachoMotor::units_to_pulses(value);
            if (pulses != position_setpoint) {
                position_setpoint = pulses;
                attributes.position_sp.write(pulses);
            }
        }

        template <Unit Unit = cm>
        Unit get_position ()
        {
            const auto pulses = attributes.position.read<int>();
            return TachoMotor::pulses_to_units<Unit>(pulses);
        }

        void set_position (const Unit auto &value)
        {
            const auto pulses = TachoMotor::units_to_pulses(value);
            attributes.position.write(pulses);
        }

        template <Unit Unit = cm>
        Unit get_speed ()
        {
            const auto pulses = attributes.speed.read<int>();
            return TachoMotor::pulses_to_units<Unit>(pulses);
        }

        template <Unit Unit = cm>
        Unit get_speed_setpoint ()
        {
            return TachoMotor::pulses_to_units<Unit>(speed_setpoint);
        }

        void set_speed_setpoint (const Unit auto &value)
        {
            auto pulses = TachoMotor::units_to_pulses(value);
            pulses = clamp(pulses, -max_speed, max_speed);
            if (pulses != speed_setpoint) {
                speed_setpoint = pulses;
                attributes.speed_sp.write(pulses);
            }
        }

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

        std::vector<std::string> get_state ()
        {
            return attributes.state.read<std::vector<std::string>>();
        }

        bool is_running ()
        {
            const auto state = get_state();
            return std::count(state.begin(), state.end(), TachoMotor::states::running);
        }

        bool is_ramping ()
        {
            const auto state = get_state();
            return std::count(state.begin(), state.end(), TachoMotor::states::ramping);
        }

        bool is_holding ()
        {
            const auto state = get_state();
            return std::count(state.begin(), state.end(), TachoMotor::states::holding);
        }

        bool is_overloaded ()
        {
            const auto state = get_state();
            return std::count(state.begin(), state.end(), TachoMotor::states::overloaded);
        }

        bool is_stalled ()
        {
            const auto state = get_state();
            return std::count(state.begin(), state.end(), TachoMotor::states::stalled);
        }

        void wait_until (const std::string_view flag)
        {
            while (true) {
                const auto state = get_state();
                if (std::count(state.begin(), state.end(), flag)) {
                    break;
                }
                attributes.state.wait();
            }
        }

        void wait_while (const std::string_view flag)
        {
            while (true) {
                const auto state = get_state();
                if (!std::count(state.begin(), state.end(), flag)) {
                    break;
                }
                attributes.state.wait();
            }
        }

        template <bool block = false>
        void on (const Unit auto &velocity)
        {
            set_speed_setpoint(velocity);
            run_command(TachoMotor::commands::run_forever);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }

        template <bool block = true, bool brake = true>
        void on_for_segment (const Unit auto &segment, const Unit auto &velocity)
        {
            set_position_setpoint(segment);
            set_speed_setpoint(velocity);

            if constexpr (brake) {
                set_stop_action(TachoMotor::stop_actions::brake);
            }
            else {
                set_stop_action(TachoMotor::stop_actions::coast);
            }

            run_command(TachoMotor::commands::run_to_relative_position);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }

        template <bool block = true, bool brake = true>
        void on_to_position (const Unit auto &position, const Unit auto &velocity)
        {
            set_position_setpoint(position);
            set_speed_setpoint(velocity);

            if constexpr (brake) {
                set_stop_action(TachoMotor::stop_actions::brake);
            }
            else {
                set_stop_action(TachoMotor::stop_actions::coast);
            }

            run_command(TachoMotor::commands::run_to_absolute_position);
            if constexpr (block) {
                wait_while(TachoMotor::states::running);
            }
        }
};

}; // namespace
