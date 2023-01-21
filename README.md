# Fazekas RoboTeam - ev3dev C++ library

Language bindings inspired by [ddemidov/ev3dev-lang-cpp](https://github.com/ddemidov/ev3dev-lang-cpp) with some added higher level functionality. It is under development, meaning it might not be complete.

First and foremost, this library serves educational purposes, and this guide is aimed to help beginners, therefore its wording tries to be casual, and it contains fully working programs in every example, which can be tried out and experimented with. Using C++ is much more complex than using Python, however it should not be forgotten, that the former runs roughly 20 times faster on an EV3 brick than MicroPython. The setting up may require assistance though.

- [Getting started](#getting-started)
    - [Compilation](#compilation)
    - [VS Code recommended settings](#vs-code-recommended-settings)
    - [Code structure](#basic-example)
- [Examples](#examples)
    - [Unregulated tank movement](#unregulated-tank-movement)
- [API overview](#api-overview)
    - [FRT::Logger](#frtlogger)
    - [FRT::TachoMotor](#frttachomotor)
    - [FRT::Sensor](#frtsensor)
    - [FRT::ColorSensor](#frtcolorsensor)
    - [Unit system](#unit-system)
    - [Other utilities](#other-utilities)

# Getting started

If you have not done it yet, install the `ev3dev` operating system onto your EV3 brick. You can download it from [here](https://www.ev3dev.org/). You require a computer with an SD card slot and an SD card.

The `main` branch in this repository is an minimal example project with our library. That means, you only need to set up the compilation process and you are ready to go. You just need to edit the source code in the `src` directory.

## Compilation

Create a file named `config.mk` and set the path of your C++20 conforming cross-compiler into a variable `CXX`.

It looks like this on my device:
```
CXX := toolchain/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-c++
```

There will be a compiler for x86-64 Linux systems in the `toolchain` directory soon. If you cannot find a compiler for your host working with `ev3dev`, we recommend trying out [crosstool-NG](https://crosstool-ng.github.io/).

You should be able now to build the project by running:
```
make
```

Command to remove build files:
```
make clean
```

## VS Code recommended settings

Install the `ev3dev.ev3dev-browser` extension.
After connecting to the brick, you can copy the built project onto the EV3 and run it by pressing F5.

It copies the whole workspace by default, however only `bin` directory is strictly neccessary. 
This can be solved by setting the `ev3devBrowser.download.include` option in the VSCode settings to `bin/**`.

If you have the extension `ms-vscode.cpptools` installed which gives you IntelliSense, set the option `C_Cpp.default.cppStandard` to `c++20` as well.

## Code structure

Here is a basic example of `main.cpp`, which will write `Hello World!` to the standard output. Do not worry if you do not understand everything, I will break it down below.

```
#include <frt/frt.hpp>

using namespace FRT::unit_literals;

// write your own functions here

int main () 
{
    std::ios_base::sync_with_stdio(false);

    // write driver code here
    FRT::Logger::info("Hello World!");
}
```

In order to use all features of the library, include the `frt/frt.hpp` convenience header at the start of your source files. It brings most of the standard library headers with itself, so you most likely do not need to worry about them.

```
#include <frt/frt.hpp>
```

The library code is enclosed in the namespace `FRT`. That means, most of the names have to be preceded by `FRT::` (e.g. `FRT::Logger`, `FRT::ColorSensor`) in the same way the standard library names are preceded by `std::` (e.g. `std::string`, `std::vector`). That way you know where each name is coming from.

Our distance and angle units are in the namespace `FRT::unit_literals`. The statement below enables their direct use (e.g. `6.2cm`, `120.0deg`).

```
using namespace FRT::unit_literals;
```

The `main` function is the entry point of our program. Code written there will get executed automatically. 

The statement below speeds up input-output a little, therefore it is recommended to be used.

```
std::ios_base::sync_with_stdio(false);
```

# Examples

## Unregulated tank movement

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

// diameter is written onto the side of the wheels
TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_B, 6.2mm);

// measure it roughly with tape, then refine during testing
const auto axle_length = 14.9cm; 

template <class DistanceUnit, class SpeedUnit>
void tank_move (const DistanceUnit distance, const SpeedUnit speed)
{
    const auto left_target = left.get_position() + distance;
    while (left_target >= left.get_position()) {
        left.on(speed);
        right.on(speed);
    }
    left.stop();
    right.stop();
}

template <class AngleUnit, class SpeedUnit>
void tank_turn (const AngleUnit angle, const SpeedUnit speed)
{
    const auto left_arc = angle * (axle_length / left.diameter);
    const auto left_target = left.get_position<_deg>() + left_arc;
    while (left_target >= left.get_position<_deg>()) {
        left.on(speed);
        right.on(-speed);
    }
    left.stop();
    right.stop();
}

int main () 
{
    std::ios_base::sync_with_stdio(false);

    // should move on a square path
    while (true) {
        tank_move(50.0cm, 500.0deg /* per second */);
        tank_turn(90.0deg, 300.0deg /* per second */);
    }
}
```

# API overview

This overview is incomplete yet, however it is mostly correct. Keep that in mind.

> Visit [Linux Kernel Drivers for ev3dev-stretch](https://docs.ev3dev.org/projects/lego-linux-drivers/en/ev3dev-stretch/) for more information about the behaviour of the underlying driver.

## FRT::Logger

Functions which make logging to the standard output easier and more akin to Python's `print()`.  Provides the log messages with timestamps, severity levels and coloring. Supports our unit system, `std::pair` and the iterable standard library containers as well.

The `FRT::log_level` variable defined in file `frt/src/config.hpp` can be configured to display messages only with a severity above a certain level. The others are discarded at compile-time, therefore they do not effect the performance at all.

Example code:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

int main ()
{
    Logger::debug("Hello World!");

    std::vector<int> vector = { 20, 30, 40 };
    Logger::info(10, vector);

    Logger::warning(50.0cm, 3.14159rad);
}
```

Output:

```
[19:37:11.032 DEBUG]   Hello World!
[19:37:11.032 INFO]    10 { 20, 30, 40 }
[19:37:11.032 WARNING] 50 cm 3.14159 rad
```

### void FRT::Logger::debug (Args... args)

### void FRT::Logger::info (Args... args)

### void FRT::Logger::warning (Args... args)

### void FRT::Logger::error (Args... args)

---

## FRT::TachoMotor (const std::string_view port, const Unit &diameter, const bool reset = true)

Constructs a TachoMotor object.

- Parameters:
    - `port`: constants `OUTPUT_A`, `OUTPUT_B`, `OUTPUT_C`, `OUTPUT_D`
    - `diameter`: diameter of wheel (e.g. `6.2mm`), usually can be found on the side of the wheel
    - `reset` (optional): if explicitly set to `false`, it will not reset the internal variables of the motor to their default values

Tips before using a motor:

1. Find out the its maximum angular velocity, and then adjust its speed accordingly. See `TachoMotor::max_speed`.
2. When using two EV3 medium motors in a tank, by default the wheels are moving into the opposite direction when a speed of the same sign is applied. This can be solved by inverting the left one's polarity. See `TachoMotor::set_polarity`.
3. Most of the times the weight distribution of a robot is imbalanced. That means, its movement could pull to some direction when the same speed is applied to both wheels, which needs fixing. See `TachoMotor::config::position_coefficient`.


### void FRT::TachoMotor::on \<bool block = false> (const Unit &velocity)

- Template parameters:
    - `block` (optional): if `true`, the call will block the thread while the motor is running
- Parameters:
    - `velocity`: distance per second, or angle per second

Rotates the motor at the rate of `velocity` forever.

Example #1:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_B, 6.2mm);

int main () 
{
    left.on<true>(30.0cm);
    right.on<true>(30.0cm);
}
```

> Oh no! This way the right motor will not get turned on, because the first call blocks the thread forever.

Example #2:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_B, 6.2mm);

int main () 
{
    left.on(30.0cm);
    right.on<true>(30.0cm);
}
```

> Setting the first call to non-blocking, the robot should now be moving forward until stopped.

---

### void FRT::TachoMotor::on_for_segment \<bool block = true, bool brake = true> (const DistanceUnit &segment, const VelocityUnit &velocity)

- Template parameters:
    - `block` (optional): if `true`, the call will block the thread while the motor is running
    - `brake` (optional): if `true`, the motor will brake instead of coasting when it stops running
- Parameters:
    - `segment`: distance
    - `velocity`: distance per second, or angle per second

Rotates the motor at the rate of `velocity` for `segment` length.

---

### void FRT::TachoMotor::on_to_position \<bool block = true, bool brake = true> (const DistanceUnit &position, const VelocityUnit &velocity)

- Template parameters:
    - `block` (optional): if `true`, the call will block the thread while the motor is running
    - `brake` (optional): if `true`, the motor will brake instead of coasting when it stops running
- Parameters:
    - `segment`: distance
    - `velocity`: distance per second, or angle per second

Rotates the motor at the rate of `velocity` to `position`.

---

### void FRT::TachoMotor::wait_until (const std::string_view flag)

- Parameters:
    - `flag`: state constant in `TachoMotor::states`

Blocks the thread until the state flag appears in the `FRT::TachoMotor::get_state()` set.

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

int main () 
{
    TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_A, 6.2mm);

    left.on(30.0cm);
    right.on(30.0cm);

    left.wait_until(TachoMotor::states::stalled);
}
```

> The robot should move forward until it bumps into something.

---

### void FRT::TachoMotor::wait_while (const std::string_view flag)

- Parameters:
    - `flag`: state constant in `TachoMotor::states`

Blocks the thread while the state flag is in the `FRT::TachoMotor::get_state()` set.

Example:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

int main () 
{
    TachoMotor motor(OUTPUT_A, 6.2mm);

    // non-blocking call + wait
    motor.on_for_segment<false>(50.0cm, 500.0deg);
    motor.wait_while(TachoMotor::states::running);

    sleep(1s);

    // blocking call
    motor.on_for_segment(50.0cm, 500.0deg);
}
```

> These two pieces of code are performing in the same way.

---

### bool FRT::TachoMotor::is_running ()

- Returns: `true` if the motor is running

---

### bool FRT::TachoMotor::is_ramping ()

- Returns: `true` if the motor is ramping up or down, and has not reached its speed setpoint yet

---

### bool FRT::TachoMotor::is_holding ()

- Returns: `true` if the motor is holding itself to a fixed position

Holding happens after the robot has stopped when the its stop action is set to `TachoMotor::stop_actions::hold`.

---

### bool FRT::TachoMotor::is_overloaded ()

- Returns: `true` if the motor cannot reach its speed setpoint because of an external force (like bumping into a wall with a tank)

---

### bool FRT::TachoMotor::is_stalled ()

- Returns: `true` if the motor is not turning when it should be

---

### Unit FRT::TachoMotor::get_position \<typename Unit = _cm> ()

- Template parameters:
    - `Unit` (optional): unit of measurement, centimeters by default
- Returns: the distance or angle the motor has moved since its last reset

Example:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

int main () 
{
    TachoMotor left(OUTPUT_A, 6.2mm), right(OUTPUT_A, 6.2mm);

    left.on(30.0cm);
    right.on(30.0cm);

    while (true) {
        const auto centimeters = left.get_position();
        const auto degrees = left.get_position<_deg>();
        Logger::info(centimeters, degrees);
    }
}
```

> The tank is moving forward and is logging its position in two different units of measurement.

---

### Unit FRT::TachoMotor::get_speed \<typename Unit = _cm> ()

- Template parameters:
    - `Unit` (optional): unit of measurement, centimeters by default
- Returns: the current velocity of the motor, units per second

---

### void FRT::TachoMotor::set_polarity (const std::string_view value)

- Parameters:
    - `value`: constant in `TachoMotor::polarities`

Sets the motors polarity to `value`, which is the direction it will rotate in.

> When using a tank equipped with medium motors, the left motor should be inverted. That way, the two wheels will go in the same direction with the same sign of speed applied.

---

### void FRT::TachoMotor::set_stop_action (const std::string_view value)

- Parameters:
    - `value`: constant in `TachoMotor::stop_actions`

Sets the behaviour of the motor when it stops running to `value`.

---

### void FRT::TachoMotor::stop ()

Runs the command `TachoMotor::commands::stop`, which stops the command currently running. The motor then behaves according to its set stop-action.

---

### void FRT::TachoMotor::reset ()

Stops the motor and resets all its inner variables.

---

## FRT::Sensor

---

## FRT::ColorSensor


---

## Unit system

---

## Other utilities

### double FRT::time ()

- Returns: current timestamp in seconds, with the resolution of microseconds

The built-in `std::chrono` library can be percieved as far too complex, especially as a beginner. The `FRT::time()` function is a simpler interface, similar to Python's `time.time()`, and is sufficient for use cases like measuring the elapsed time between two events during debugging.

---

### void FRT::sleep (const T &duration)

- Parameters:
    - `duration`: duration to sleep

Shorthand for `std::this_thread::sleep_for()`. Accepts the units of measurement of `std::chrono_literals`. Similar to Python's `time.sleep()`.

Example code:

```
#include <frt/frt.hpp>

using namespace FRT;
using namespace FRT::unit_literals;

int main () 
{
    const auto start = time();
    sleep(500ms);
    const auto end = time();
    
    Logger::info("Seconds elapsed:", end - start);
}
```

Output:

```
[20:04:50.807 INFO]    Seconds elapsed: 0.50011
```

---

### auto FRT::clamp (const T &value, const T &min, const T &max)

- Returns: `value`, if `value` is between `min` and `max`, `min`, if it is smaller than `min`, `max` if it is bigger than `max˙

Shorthand for `std::min(std::max(value, min), max)`. When running controlled movement, it can be used to restrict the motor's angular velocity into the supported range and avoid misbehaviour and errors.

---
