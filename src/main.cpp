#include "logger.hpp"
#include "utility.hpp"
#include "motor.hpp"
#include "sensor.hpp"
#include "tank.hpp"
#include "scheduler.hpp"

using namespace FRT;

class Robot : public Tank
{
    public:
        using Tank::Tank;

        void run ()
        {
            left_motor.set_polarity(TachoMotor::polarities::inversed);
            left_motor.config.position_coefficient = 1.0075;

            on_for_segment(150.0cm, 1020.0deg, 1020.0deg);
        }
};

int main () 
{
    /*auto left = TachoMotor(OUTPUT_A, 6.2mm);
    auto right = TachoMotor(OUTPUT_B, 6.2mm);
    Robot(left, right).run();*/

    auto scheduler = Scheduler(100);

    int *i_ = new int;
    *i_ = 0;
    Task task1 = Task(
        [i_] (Scheduler &s) mutable {
            int &i = *i_;

            if (i < 500) {
                i++;
                FRT::Logger::info(i);
            } else {
                s.cancel();
            }
        }
    , {});

    int *k_ = new int;
    *k_ = 0;
    Task task2 = Task(
        [k_] (Scheduler &s) mutable {
            int &k = *k_;

            if (k < 500) {
                k++;
                FRT::Logger::info(k);
            } else {
                s.cancel();
            }
        }
    , {});

    scheduler.create_task(task1);
    scheduler.create_task(task2);
    scheduler.run();
}
