#pragma once

#include "file.hpp"
#include "logger.hpp"
#include "utility.hpp"

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

        FRT::File attribute (const std::string_view filename) {
            return FRT::File(prefix + (std::string)filename);
        }

    public:
        Device (const std::string &dir, const std::string_view address) noexcept
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
                        if (found_address.find(address) == 0) {
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

        Device (const std::string &path) noexcept
        {
            prefix = SYSFS_DIR + path;
            FRT::Logger::info("Device::connect - absolute path:", path);
        }

        Device (const Device &) = delete;
        virtual ~Device () {};
};



}; // namespace
