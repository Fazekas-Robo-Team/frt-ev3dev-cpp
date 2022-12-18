#pragma once

#include "container.hpp"

#include <functional>
#include <thread>
#include <cmath>

namespace FRT
{

class Scheduler;

class Task
{
    public:
        typedef std::function<void(Scheduler&)> Function;

        Function function;
        FRT::Vector<void*> arguments;
        bool cancelled = false;

        Task (const Function &function, const FRT::Vector<void*> &arguments) : function(function), arguments(arguments)
        {
            
        }
};

class Scheduler
{
    private:
        int tickspeed;
        FRT::List<Task> tasks, incoming_tasks;
        FRT::List<Task>::iterator current_task;

    public:
        Scheduler (const int tickspeed)
        :   tickspeed(tickspeed)
        {}

        [[noreturn]] void run ()
        {
            using namespace std::chrono;
            using namespace std::chrono_literals;

            const auto tick_frame = nanoseconds(lround(1e9 / tickspeed));

            while (true) {
                const auto start_time = system_clock::now();
                tick();
                const auto end_time = system_clock::now();
                const auto elapsed = duration_cast<nanoseconds>(end_time - start_time);
                if (elapsed < tick_frame) {
                    std::this_thread::sleep_for(tick_frame - elapsed);
                }
            }
        }

        void tick ()
        {
            // running tasks
            current_task = tasks.begin();
            while (current_task != tasks.end()) {
                current_task->function(*this);
                current_task++;
            }

            // cleaning up 
            current_task = tasks.begin();
            while (current_task != tasks.end()) {
                if (current_task->cancelled) {
                    current_task = tasks.erase(current_task);
                }
                else {
                    current_task++;
                }
            }

            // recieving incoming tasks
            tasks.splice(tasks.begin(), incoming_tasks);
        }

        void create_task (Task &task)
        {
            incoming_tasks.push_back(task);
        }

        void cancel ()
        {
            current_task->cancelled = true;
        }
};

}; // namespace