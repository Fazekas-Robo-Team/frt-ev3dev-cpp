#pragma once

#include "config.hpp"
#include "logger.hpp"
#include "container.hpp"

#include <fstream>
#include <mutex>
#include <sstream>

#include <errno.h>

#include <sys/inotify.h>
#include <unistd.h>

namespace FRT
{

class File
{
    protected:
        /// @brief Full filesystem path of the file.
        std::string path;

        /// @brief Encapsulated std::ifstream object.
        std::ifstream input_stream;

        /// @brief Encapsulated std::ofstream object.
        std::ofstream output_stream;

        /// @brief Intended to make the streams thread-safe. Methods doing I/O should always lock it.
        mutable std::recursive_mutex mutex;

        int file_descriptor;

        /// @brief Ensures that input_stream is open and points to the beginning of the file.
        /// @tparam silent Option to suppress error messages. Useful when probing for devices. Defaults to false.
        template <bool silent = false>
        void ensure_input ()
        {
            if (!input_stream.is_open()) {
                input_stream.open(path);

                if (!input_stream.is_open()) {
                    if constexpr (!silent) {
                        Logger::error("File::ensure_input - cannot open", path);
                    }
                }
            }
            // getting rid of error state flags like EOF
            input_stream.clear();
            // changing the read position to the beginning of the file
            input_stream.seekg(0, std::ios::beg);
        }

        /// @brief Ensures that output_stream is open.
        void ensure_output ()
        {
            if (!output_stream.is_open()) {
                output_stream.rdbuf()->pubsetbuf(NULL, 0);
                output_stream.open(path);

                if (!output_stream.is_open()) {
                    Logger::error("File::ensure_input - cannot open", path);
                }
            }

            output_stream.clear();
        }

        /// @brief Template specialization workaround for reading sets. Used by File::read.
        /// @param attempts Passed to File::read_line.
        /// @return 
        Set<std::string> read_set (int attempts = 2)
        {
            const auto line = read_line(attempts);

            std::pmr::set<std::string> result;
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

            return { std::move(result) };
        }

    public:
        File (const std::string &path) : path(path) {}

        virtual ~File () {}

        /// @brief Reads data from the beginning of the file. Reading strings stops at any whitespace, see File::read_line if needed.
        /// @tparam T Type of data to read. Arithmetic types, std::string and FRT::Set are typical.
        /// @tparam silent Option to suppress error messages. Useful when probing for devices. Defaults to false.
        /// @param attempts Determines how many times to retry in case of failure. Defaults to two.
        template <typename T, bool silent = false>
        T read (int attempts = 2)
        {
            if constexpr (std::is_same_v<T, Set<std::string>>) {
                return read_set(attempts);
            } else {
                if (attempts == 0) {
                    if constexpr (!silent) Logger::error("File::read - attempts reached zero");
                }

                const auto lock = std::scoped_lock(mutex);
                ensure_input<silent>();

                try {
                    T result;
                    input_stream >> result;
                    return result;
                } 
                catch (...) {
                    if constexpr (!silent) Logger::warning("File::read - read failed");
                    input_stream.close();
                    input_stream.clear();
                    return read<T>(attempts - 1);
                }
            }
        }

        /// @brief Reads a whole line from the beginning of the file.
        /// @param attempts Determines how many times to retry in case of failure. Defaults to two.
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
                return read_line(attempts - 1);
            }
        }

        /// @brief Writes data to the file.
        /// @tparam T Type of data to write. Arithmetic types and std::string are typical.
        /// @param attempts Determines how many times to retry in case of failure. Defaults to two.
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
            return write<T>(value, attempts - 1);
        }

        void wait ()
        {
            const auto file_descriptor = inotify_init();
            const auto watch_descriptor = inotify_add_watch(file_descriptor, path.c_str(), IN_MODIFY);
            ::read(file_descriptor, 0, 0);
        }
};

} // namespace