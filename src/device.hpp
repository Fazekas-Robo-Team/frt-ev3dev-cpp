#pragma once

#include "file.hpp"
#include "logger.hpp"

#include <dirent.h>
#include <cstring>
#include <cmath>

#define INPUT_1 "ev3-ports:in1"
#define INPUT_2 "ev3-ports:in2"
#define INPUT_3 "ev3-ports:in3"
#define INPUT_4 "ev3-ports:in4"

#define OUTPUT_A "ev3-ports:outA"
#define OUTPUT_B "ev3-ports:outB"
#define OUTPUT_C "ev3-ports:outC"
#define OUTPUT_D "ev3-ports:outD"

#define SYSFS_DIR "/sys/class/"

namespace FRT
{

class Device
{
    protected:
        std::string prefix;

        FRT::File attribute (const std::string &filename) {
            return FRT::File(prefix + filename);
        }

    public:
        Device (const std::string &dir, const std::string &address) noexcept
        {
            DIR *dfd;
            struct dirent *dp; // directory entry

            // opening directory
            if ((dfd = opendir((SYSFS_DIR + dir).c_str())) != nullptr) {
                // iterating through directory
                while ((dp = readdir(dfd)) != nullptr) {
                    try {
                        prefix = SYSFS_DIR + dir + dp->d_name + '/';
                        const auto found_address = FRT::File(prefix + "address").read<std::string, true>();
                        if (address == found_address) {
                            closedir(dfd);
                            FRT::Logger::info("Device::connect - success, address:", address, ", prefix:", prefix);
                            return;
                        }
                    } catch (...) {}
                    prefix.clear();
                }
                closedir(dfd);
            }

            FRT::Logger::error("Device::connect - failed, address:", address);
        }

        virtual ~Device () {};
};

class SensorInterface : public Device
{
    public:
        using Device::Device;

        FRT::File address = attribute("address");
        FRT::File bin_data = attribute("bin_data");
        FRT::File bin_data_format = attribute("bin_data_format");
        FRT::File command = attribute("command");
        FRT::File commands = attribute("commands");
        FRT::File direct = attribute("direct");
        FRT::File decimals = attribute("decimals");
        FRT::File driver_name = attribute("driver_name");
        FRT::File fw_version = attribute("fw_version");
        FRT::File mode = attribute("mode");
        FRT::File modes = attribute("modes");
        FRT::File num_values = attribute("num_values");
        FRT::File poll_ms = attribute("poll_ms");
        FRT::File units = attribute("units");
        FRT::File text_value = attribute("text_value");
        FRT::File value[5] = {
            { "value0" }, 
            { "value1" }, 
            { "value2" }, 
            { "value3" }, 
            { "value4" },
        };
};

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

class Sensor
{
    public:
        SensorInterface attributes;
        Sensor (const std::string &port) : attributes("lego-sensor/", port) {} 
};

struct _cm { _cm (double value) : value(value) {} double value; };
struct _degrees { _degrees (double value) : value(value) {} double value; };
struct _radian { _radian (double value) : value(value) {} double value; };

_cm operator""cm (const long double value) { return _cm(value); }
_degrees operator""deg (const long double value) { return _degrees(value); }
_radian operator""rad (const long double value) { return _radian(value); }
_cm operator""cm (const unsigned long long value) { return _cm(value); }
_degrees operator""deg (const unsigned long long value) { return _degrees(value); }
_radian operator""rad (const unsigned long long value) { return _radian(value); }

class TachoMotor
{
    public:
        TachoMotorInterface attributes;

        // constant attributes
        const double diameter;
        const std::string port;
        const FRT::Set<std::string> supported_modes;
        const int pulses_per_rotation;
        const std::string driver_name;
        const int max_speed;
        const FRT::Set<std::string> supported_stop_actions;

        TachoMotor (const std::string &port, const _cm diameter) 
        :   attributes("tacho-motor/", port),
            diameter(diameter.value),
            port(attributes.address.read<std::string>()),
            supported_modes(attributes.commands.read<FRT::Set<std::string>>()),
            pulses_per_rotation(attributes.count_per_rot.read<int>()),
            driver_name(attributes.driver_name.read<std::string>()),
            max_speed(attributes.max_speed.read<int>()),
            supported_stop_actions(attributes.stop_actions.read<FRT::Set<std::string>>())
        {}

        TachoMotor (const TachoMotor &) = default;

        static const struct {
            const std::string run_forever = "run-forever";
            const std::string run_to_absolute_position = "run-to-abs-pos";
            const std::string run_to_relative_position = "run-to-rel-pos";
            const std::string run_timed = "run-timed";
            const std::string run_direct = "run-direct";
            const std::string stop = "stop";
            const std::string reset = "reset";
        } modes;

        static const struct {
            const std::string normal = "normal";
            const std::string inversed = "inversed";
        } polarities;

        static const struct {
            const std::string running = "running";
            const std::string ramping = "ramping";
            const std::string holding = "holding";
            const std::string overloaded = "overloaded";
            const std::string stalled = "stalled";
        } states;

        static const struct {
            const std::string coast = "coast";
            const std::string brake = "brake";
            const std::string hold = "hold";
        } stop_actions;

        struct {
            double position_coefficient = 1;
        } config;

        double pulse_cast (const _degrees value) { return value.value / 360 * pulses_per_rotation; }
        double pulse_cast (const _radian value) { return value.value / acos(-1) * pulses_per_rotation; }
        double pulse_cast (const _cm value) { return value.value / diameter / acos(-1) * pulses_per_rotation; }
        double pulse_cast (const double value) { return value; }

        std::string get_mode ()
        {
            return attributes.command.read<std::string>();
        }

        void set_mode (const std::string &value) 
        {
            attributes.command.write(value);
        }

        std::string get_stop_action ()
        {
            return attributes.stop_action.read<std::string>();
        }

        void set_stop_action (const std::string &value) 
        {
            attributes.stop_action.write(value);
        }

        int get_duty_cycle () 
        {
            return attributes.duty_cycle.read<int>();
        }

        int get_duty_cycle_setpoint ()
        {
            return attributes.duty_cycle_sp.read<int>();
        }

        void set_duty_cycle_setpoint (const int value)
        {
            return attributes.duty_cycle_sp.write(value);
        }

        std::string get_polarity () 
        {
            return attributes.polarity.read<std::string>();
        }

        void set_polarity (const std::string &value) 
        {
            attributes.polarity.write<std::string>(value);
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

        int get_position_setpoint ()
        {
            return attributes.position_sp.read<int>();
        }

        template <typename Unit>
        void set_position_setpoint (const Unit &value)
        {
            const int pulses = pulse_cast(value) / config.position_coefficient;
            attributes.position_sp.write(pulses);
        }

        double get_position ()
        {
            return config.position_coefficient * attributes.position.read<int>();
        }

        template <typename Unit>
        void set_position (const Unit &value)
        {
            const int pulses = pulse_cast(value) / config.position_coefficient;
            attributes.position.write(pulses);
        }

        int get_speed ()
        {
            return attributes.speed.read<int>();
        }

        int get_speed_setpoint ()
        {
            return attributes.speed_sp.read<int>();
        }

        template <typename Unit>
        void set_speed_setpoint (const Unit &value)
        {
            const int pulses = pulse_cast(value);
            attributes.speed_sp.write(pulses);
        }

        int get_ramp_up_setpoint ()
        {
            return attributes.ramp_up_sp.read<int>();
        }

        void set_ramp_up_setpoint (const int milliseconds)
        {
            attributes.ramp_up_sp.write(milliseconds);
        }

        int get_ramp_down_setpoint ()
        {
            return attributes.ramp_down_sp.read<int>();
        }

        void set_ramp_down_setpoint (const int milliseconds)
        {
            attributes.ramp_down_sp.write(milliseconds);
        }

        std::string get_state ()
        {
            return attributes.state.read<std::string>();
        }
};

}
