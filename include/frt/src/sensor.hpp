#pragma once

#include "device.hpp"

namespace FRT
{

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
        FRT::File value[8] = {
            { "value0" }, 
            { "value1" }, 
            { "value2" }, 
            { "value3" }, 
            { "value4" },
            { "value5" },
            { "value6" },
            { "value7" },
        };
};

class Sensor
{
    public:
        SensorInterface attributes;
        Sensor (const std::string &port) : attributes("lego-sensor/", port) {} 
};

}; // namespace
