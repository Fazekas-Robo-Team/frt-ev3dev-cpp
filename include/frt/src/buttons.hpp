#pragma once

#include "logger.hpp"

#include <string_view>
#include <vector>
#include <cmath>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

namespace FRT
{

class Button;

class Buttons
{
    const int file_descriptor;
    std::vector<unsigned int> state;

    static constexpr char path[] = "/dev/input/by-path/platform-gpio_keys-event";

    void read () const
    {
        const int error = ioctl(file_descriptor, EVIOCGKEY(state.size()), state.data());
        if (error < 0) {
            Logger::error("Button::read - ioctl read failed");
        }
    }

    template <bool read = true>
    bool is_pressed (const int button)
    {
        if constexpr (read) {
            Buttons::read();
        }
        return state[button / sizeof(unsigned long) / 8] & (1 << (button % (sizeof(unsigned long) * 8)));
    }

    void wait_until (const int button)
    {
        while (!is_pressed(button)) {
            // busy wait
        }
    }

    struct Button
    {
        friend class Buttons;

        const int bit;
        Buttons &buttons;

        Button (const int bit, Buttons &buttons)
        : bit(bit),
          buttons(buttons)
        {}

        public:
            bool is_pressed () const
            {
                return buttons.is_pressed(bit);
            }

            void wait_until () const
            {
                buttons.wait_until(bit);
            }
    };

    public:
        const Button left = { KEY_LEFT, *this };
        const Button right = { KEY_RIGHT, *this };
        const Button up = { KEY_UP, *this };
        const Button down = { KEY_DOWN, *this };
        const Button enter = { KEY_ENTER, *this };
        const Button back = { KEY_BACK, *this };
        
        Buttons ()
        : file_descriptor(open(path, O_RDONLY)),
          state(std::ceil(KEY_CNT / sizeof(unsigned long) / 8), 0)
        {}

        ~Buttons ()
        {
            if (file_descriptor != -1) {
                close(file_descriptor);
            }
        }
};

} // namespace