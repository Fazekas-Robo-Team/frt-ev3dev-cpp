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
            attribute("value0"), 
            attribute("value1"), 
            attribute("value2"), 
            attribute("value3"), 
            attribute("value4"),
            attribute("value5"),
            attribute("value6"),
            attribute("value7"),
        };
};

struct ColorBase
{
    const int value;
    constexpr ColorBase (const int value) : value(value) 
    {}

    operator int () const
    {
        return value;
    }
};

class Sensor
{
    protected:
        std::string mode = "";

    public:
        SensorInterface attributes;

        // constant attributes
        const std::string port;
        const std::vector<std::string> supported_modes;
        const std::vector<std::string> supported_commands;
        const std::string driver_name;
        const std::string firmware_version;
        
        Sensor (const std::string_view port) 
        :   attributes("lego-sensor/", port),
            port(attributes.address.read<std::string>()),
            supported_modes(attributes.modes.read<std::vector<std::string>>()),
            supported_commands(attributes.commands.read<std::vector<std::string>>()),
            driver_name(attributes.driver_name.read<std::string>()),
            firmware_version(attributes.fw_version.read<std::string>())
        {
            get_mode();
        } 

        void run_command (const std::string_view command)
        {
            attributes.command.write(command);
        }

        void set_mode (const std::string_view value)
        {
            if (value != mode) {
                attributes.mode.write(value);
                mode = value;
            }
        }

        std::string get_mode () 
        {
            if (mode == "") {
                mode = attributes.mode.read<std::string>();
            }
            return mode;
        }

        int get_decimals () 
        {
            return attributes.decimals.read<int>();
        }

        int get_number_of_values () 
        {
            return attributes.num_values.read<int>();
        }

        void set_poll_ms (const int ms)
        {
            attributes.poll_ms.write(ms);
        }

        int get_poll_ms ()
        {
            return attributes.poll_ms.read<int>();
        }
};

class ColorSensor : public Sensor
{
    public:
        ColorSensor (const std::string_view port)
        : Sensor(port)
        {}

        struct modes
        {
            static constexpr std::string_view reflected_light_intensity = "COL-REFLECT";
            static constexpr std::string_view ambient_light_intensity = "COL-AMBIENT";
            static constexpr std::string_view color = "COL-COLOR";
            static constexpr std::string_view raw_reflected_light_intensity = "REF-RAW";
            static constexpr std::string_view raw_rgb = "RGB-RAW";
        };

        struct colors;

        struct Color : public ColorBase
        {
            using ColorBase::ColorBase;

            friend std::ostream &operator<< (std::ostream &s, const Color color)
            {
                static const std::vector<std::string_view> display = {
                    "none",
                    "black",
                    "blue",
                    "green",
                    "yellow",
                    "red",
                    "white",
                    "brown",
                };
                return s << display.at(color.value);
            }
        };

        int get_red ()
        {
            set_mode(modes::raw_rgb);
            return attributes.value[0].read<int>();
        }

        int get_green ()
        {
            set_mode(modes::raw_rgb);
            return attributes.value[1].read<int>();
        }

        int get_blue ()
        {
            set_mode(modes::raw_rgb);
            return attributes.value[2].read<int>();
        }

        int get_reflected_light_intensity ()
        {
            set_mode(modes::reflected_light_intensity);
            return attributes.value[0].read<int>();
        }

        int get_ambient_light_intensity ()
        {
            set_mode(modes::ambient_light_intensity);
            return attributes.value[0].read<int>();
        }

        Color get_color ()
        {
            set_mode(modes::color);
            const int value = attributes.value[0].read<int>();
            return Color(value);
        }
};

struct ColorSensor::colors {
    static constexpr ColorSensor::Color none = { 0 };
    static constexpr ColorSensor::Color black = { 1 };
    static constexpr ColorSensor::Color blue = { 2 };
    static constexpr ColorSensor::Color green = { 3 };
    static constexpr ColorSensor::Color yellow = { 4 };
    static constexpr ColorSensor::Color red = { 5 };
    static constexpr ColorSensor::Color white = { 6 };
    static constexpr ColorSensor::Color brown = { 7 };
};

class HTColorSensorV2 : public Sensor
{
    public:
        HTColorSensorV2 (const std::string_view port)
        : Sensor(port)
        {}

        struct modes
        {
            static constexpr std::string_view color = "COLOR";
            static constexpr std::string_view rgbw = "RGB";
            static constexpr std::string_view normalized_rgbw = "NORM";
            static constexpr std::string_view passive_rgbw = "PASSIVE";
            static constexpr std::string_view raw_rgbw = "RAW";
        };

        struct commands
        {
            static constexpr std::string_view frequency_50hz = "50HZ";
            static constexpr std::string_view frequency_60hz = "60HZ";
        };

        struct colors;

        struct Color : public ColorBase
        {
            using ColorBase::ColorBase;

            friend std::ostream &operator<< (std::ostream &s, const Color color)
            {
                static const std::vector<std::string_view> display = {
                    "black",
                    "purple",
                    "blue",
                    "cyan",
                    "green",
                    "olive",
                    "yellow",
                    "gold",
                    "orange",
                    "red",
                    "magenta",
                    "violet",
                    "lavender",
                    "mint",
                    "cream",
                    "salmon",
                    "pink",
                    "white",
                };
                return s << display[color.value];
            }
        };

        int get_red ()
        {
            set_mode(modes::rgbw);
            return attributes.value[0].read<int>();
        }

        int get_green ()
        {
            set_mode(modes::rgbw);
            return attributes.value[1].read<int>();
        }

        int get_blue ()
        {
            set_mode(modes::rgbw);
            return attributes.value[2].read<int>();
        }

        int get_white ()
        {
            set_mode(modes::rgbw);
            return attributes.value[3].read<int>();
        }

        Color get_color ()
        {
            set_mode(modes::color);
            const int value = attributes.value[0].read<int>();
            return Color(value);
        }
};

struct HTColorSensorV2::colors {
    static constexpr HTColorSensorV2::Color black = { 0 };
    static constexpr HTColorSensorV2::Color purple = { 1 };
    static constexpr HTColorSensorV2::Color blue = { 2 };
    static constexpr HTColorSensorV2::Color cyan = { 3 };
    static constexpr HTColorSensorV2::Color green = { 4 };
    static constexpr HTColorSensorV2::Color olive = { 5 };
    static constexpr HTColorSensorV2::Color yellow = { 6 };
    static constexpr HTColorSensorV2::Color gold = { 7 };
    static constexpr HTColorSensorV2::Color orange = { 8 };
    static constexpr HTColorSensorV2::Color red = { 9 };
    static constexpr HTColorSensorV2::Color magenta = { 10 };
    static constexpr HTColorSensorV2::Color violet = { 11 };
    static constexpr HTColorSensorV2::Color lavender = { 12 };
    static constexpr HTColorSensorV2::Color mint = { 13 };
    static constexpr HTColorSensorV2::Color cream = { 14 };
    static constexpr HTColorSensorV2::Color salmon = { 15 };
    static constexpr HTColorSensorV2::Color pink = { 16 };
    static constexpr HTColorSensorV2::Color white = { 17 };
};

}; // namespace
