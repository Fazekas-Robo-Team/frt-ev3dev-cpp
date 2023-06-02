#pragma once

#include "device.hpp"

namespace FRT
{

class LEDInterface : public Device
{
    public:
        using Device::Device;

        File brightness = attribute("brightness");
        File max_brightness = attribute("max_brightness");
        File trigger = attribute("trigger");
        File delay_on = attribute("delay_on");
        File delay_off = attribute("delay_off");
};

class LED
{
    struct LEDColor
    {
        LEDInterface attributes;

        LEDColor (const std::string &path)
        : attributes(path)
        {}

        int get_max_brightness ()
        {
            return attributes.max_brightness.read<int>();
        }

        int get_brightness ()
        {
            return attributes.brightness.read<int>();
        }

        void set_brightness (const int brightness)
        {
            attributes.brightness.write(brightness);
        }

        std::vector<std::string> get_supported_triggers ()
        {
            return attributes.trigger.read<std::vector<std::string>>();
        }

        void set_trigger (const std::string_view trigger)
        {
            attributes.trigger.write(trigger);
        }

        int get_delay_on ()
        {
            return attributes.delay_on.read<int>();
        }

        void set_delay_on (const int delay_on)
        {
            attributes.delay_on.write(delay_on);
        }

        int get_delay_off ()
        {
            return attributes.delay_off.read<int>();
        }

        void set_delay_off (const int delay_off)
        {
            attributes.delay_off.write(delay_off);
        }
    };

    public:
        LEDColor red_left = { "leds/led0:red:brick-status/" };
        LEDColor green_left = { "leds/led0:green:brick-status/" };
        LEDColor red_right = { "leds/led1:red:brick-status/" };
        LEDColor green_right = { "leds/led1:green:brick-status/" };
        
};

} // namespace
