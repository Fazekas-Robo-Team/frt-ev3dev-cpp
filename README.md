# Fazekas RoboTeam - ev3dev C++ library

Language bindings inspired by [ddemidov/ev3dev-lang-cpp](https://github.com/ddemidov/ev3dev-lang-cpp) with some added higher level functionality. It is under development, meaning it might not be complete.

- [Getting started](#getting-started)
    - [Compilation](#compilation)
    - [VS Code recommended settings](#vs-code-recommended-settings)
    - [Code structure](#basic-example)
- [Examples](#examples)
    - [Unregulated tank movement](#unregulated-tank-movement)
- [Features](#features)
    - [FRT::Logger](#frtlogger)
    - [FRT::TachoMotor](#frttachomotor)
    - [FRT::Sensor](#frtsensor)
    - [FRT::ColorSensor](#frtcolorsensor)
    - [Unit system](#unit-system)
    - [Other utilities](#other-utilities)


# Getting started

If you have not done it yet, install the `ev3dev` operating system onto your EV3 brick. You can download it from [here](https://www.ev3dev.org/).

The `main` branch in this repo is an minimal example project with our library, that means you only need to set up the compilation process and you are ready to go. Just edit the source code in the `src` directory.

## Compilation

Create a file named `config.mk` and set the path of your C++20 conforming cross-compiler into a variable `CXX`.

It looks like this on my device:
```
CXX := toolchain/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-c++
```

There will be a compiler for x86-64 Linux systems in the `toolchain` directory soon by default. If you cannot find a compiler for your host working with `ev3dev`, we recommend trying out [crosstool-NG](https://crosstool-ng.github.io/).

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

This guide aims to help whom just started learning C++.

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

The statement below speeds up input-output a little, therefore it is recommended to be used at the start of `main`.

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

# Reference

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

Example output:

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

## FRT::TachoMotor

---

## FRT::Sensor

---

## FRT::ColorSensor


---

## Unit system

---

## Other utilities


