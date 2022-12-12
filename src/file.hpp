#pragma once

#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"

#include <fstream>
#include <mutex>
#include <sstream>

#include <errno.h>

namespace FRT
{

class File
{
    private:
        std::string path;
        std::ifstream input_stream;
        std::ofstream output_stream;
        mutable std::recursive_mutex mutex;

        void ensure_input ()
        {
            if (!input_stream.is_open()) {
                input_stream.open(path);
            }
            #ifdef SAFE
            if (!input_stream.is_open()) {
                Logger::error("File::ensure_input - cannot open", path);
            }

            input_stream.clear();
            #endif
            input_stream.seekg(0, std::ios::beg);
        }

        void ensure_output ()
        {
            if (!output_stream.is_open()) {
                output_stream.rdbuf()->pubsetbuf(NULL, 0);
                output_stream.open(path);
            } 

            if (!output_stream.is_open()) {
                Logger::error("File::ensure_input - cannot open", path);
            }

            output_stream.clear();
        }

        // template specialization workaround for reading sets
        public: Set<std::string> read_set (int attempts = 2)
        {
            const auto line = read_line(attempts);

            Set<std::string> result;
            std::string buffer;
            
            for (const char current_char : line) {
                if (current_char == ' ') {
                    if (!buffer.empty()) {
                        result.insert(buffer);
                        buffer.clear();
                    }
                    continue;
                }
                buffer += current_char;
            }
            
            if (!buffer.empty()) {
                result.insert(buffer);
            }

            return result;
        }

    public:
        File (const std::string &path) : path(path) {}

        template <typename T>
        T read (int attempts = 
        #ifdef SAFE
        2
        #else
        1
        #endif
        )
        {
            if constexpr (std::is_same_v<T, Set<std::string>>) {
                return read_set(attempts);
            } else {
                #ifdef SAFE
                if (attempts == 0) {
                    Logger::error("File::read - attempts reached zero");
                }
                #endif

                const auto lock = std::scoped_lock(mutex);
                ensure_input();
                #ifdef SAFE
                try {
                #endif
                    T result;
                    input_stream >> result;
                    return result;
                #ifdef SAFE
                } 
                catch (...) {
                    Logger::warning("File::read - read failed");
                    input_stream.close();
                    input_stream.clear();
                    return read<T>(attempts - 1);
                }
                #endif
            }
        }

        std::string read_line (int attempts = 2)
        {
            if (attempts == 0) {
                Logger::error("File::read_line - attempts reached zero");
            }

            const auto lock = std::scoped_lock(mutex);
            ensure_input();

            try {
                std::string result;
                std::getline(input_stream, result);
                return result;
            } 
            catch (...) {
                Logger::warning("File::read_line - read failed");
                input_stream.close();
                input_stream.clear();
                return read_line(attempts - 1);
            }
        }

        template <typename T>
        void write (const T &value, int attempts = 2)
        {
            if (attempts == 0) {
                Logger::error("File::write - attempts reached zero");
            }

            const auto lock = std::scoped_lock(mutex);
            ensure_output();

            if (output_stream << value) {
                return;
            }

            Logger::warning("File::write - write failed, ERRNO:", errno);
            output_stream.close();
            output_stream.clear();
            return write<T>(value, attempts - 1);
        }
};

} // namespace