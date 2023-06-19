#pragma once

#include <cstdlib>
#include <string>

namespace FRT
{

namespace Sound
{

template <bool async = false>
inline void beep (const std::string args = "")
{   
    std::string command = "beep " + args;
    if constexpr (async) {
        command += " &";
    }
    const int error = std::system(command.c_str());
    if (error < 0) {
        // todo: handle error
    }
}

template <bool async = false>
inline void beep (const int frequency, const int duration)
{
    beep<async>("-f " + std::to_string(frequency) + " -l " + std::to_string(duration));
}

} // namespace Sound

} // namespace FRT
