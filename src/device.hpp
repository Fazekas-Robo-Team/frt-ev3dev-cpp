#pragma once

#include "file.hpp"
#include "logger.hpp"

#include <dirent.h>
#include <cstring>

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
        FRT::File speed_up = attribute("speed_up");
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
        SensorInterface attribute;
        Sensor (const std::string &port) : attribute("lego-sensor/", port) {} 
};

class TachoMotor
{
    public:
        TachoMotorInterface attribute;
        TachoMotor (const std::string &port) : attribute("tacho-motor/", port) {} 
};

}
