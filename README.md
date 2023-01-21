## How to use the library

If you have not done it yet, install the `ev3dev` operating system onto your EV3 brick. You can find information on the link below.

https://www.ev3dev.org/

The `main` branch in this repo is an minimal example project with our library, that means you only need to set up the compilation process and you are ready to go. Just edit the source code in the `src` directory.

## Compilation

Create a file named `config.mk` and set the path of your C++20 conforming cross-compiler into a variable `CXX`.

It looks like this on my Macbook:
```
CXX := toolchain/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-c++
```

There is a compiler for x86-64 Linux systems in the `toolchain` directory by default. If you cannot find a compiler for your host working with `ev3dev`, read the section *Creating a cross-compiler*.

You should be able now to build the project by running:
```
make
```

Command to remove build files:
```
make clean
```

## VSCode recommended settings

Install the `ev3dev.ev3dev-browser` extension.
After connecting to the brick, you can copy the built project onto the EV3 and run it by pressing F5.

It copies the whole workspace by default, however only `bin` directory is strictly neccessary. 
This can be solved by setting the `ev3devBrowser.download.include` option in the VSCode settings to `bin/**`.

If you have the extension `ms-vscode.cpptools` installed which gives you IntelliSense, set the option `C_Cpp.default.cppStandard` to `c++20` as well.

## Creating a cross compiler

*Work in progress*

https://crosstool-ng.github.io/

https://sourceware.org/bugzilla/show_bug.cgi?id=29564