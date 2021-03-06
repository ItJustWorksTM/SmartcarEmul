# SmartcarEmul ![Emul C++ CI](https://github.com/DIT112-V20/group-09/workflows/Emul%20C++%20CI/badge.svg)
_A [Smartcar](https://github.com/platisd/smartcar_shield) emulator_

[![IMAGE ALT TEXT](https://aerostun.codes/smce_cover_vid.jpg)](https://youtu.be/1SiTh60qjD0 "SmartCarEmul Demo - An emulator for devices based on the SmartCar.")

### Description

SmartcarEmul is an emulator for devices based on the SmartCar.
  
Through the emulator, a user is able to test their Arduino code with a vehicle in a virtual world. 
Various hardware devices are integrated so that a majority of user code can run without any modification.
Uith the help of game engine, one can take control of a camera in the virtual environment and observe it
under whatever angle they desire.
  
Via the emulator, users would also be able to test the capability of their code
without interacting with the real car. This could be done remotely through the mobile 
application that would be provided alongside the emulator.
  
To make the aforementioned possible, the emulator would use the SmartCar Shield library 
to control various interactions with the SmartCar, through which we would be able to properly 
showcase the full potential of the SmartCar library by utilising multiple features from it. 
Furthermore, since we would not depending on any specific host hardware,
our emulator would demonstrate the hardware flexibility of the library.

### Rationale

In 2019, several teams of students in the DIT112 course of Gothenburg 
University demonstrated that the Smartcar platform allows its users to solve a
large number of problems.
In an effort to allow potential users to run the students' code themselves and
even try to see if the platform is a feasible option for their needs, we have
come up with an emulator - a virtual world which lets its users develop proofs
of concept on the Smartcar platform without the need for access to the hardware.

[//]: # (###### COVID-19 pandemic)

[//]: # (In 2020, the COVID-19 pandemic meant that students would stay at home and)
[//]: # (should not meet. This caused them great trouble when it comes to testing their)
[//]: # (solutions on the Smartcar hardware since they would not be able to meet up)
[//]: # (around the car and test out their code. This is what sparked the idea to)
[//]: # (develop an emulator for the Smartcar platform, so that the students of the next)
[//]: # (years would be able to follow the course without the need for hardware access)
[//]: # (early in the development process.)

### Supported hosts

- Linux-based systems (feel free to try out other Un*x systems)
- Microsoft Windows (NT kernel)
- macOS

### Build dependencies

- CMake >= 3.13
- A C++2a capable toolchain
- Prebuilt Urho3D (1.7.1)
- This git repository's submodules in `thirdparty/`

### Supported compilers

- GNU GCC >= 9
- Clang/LLVM >= 10
- MSVC >=? 19.26


### Build instructions

```shell script
git clone --recurse-submodules \
    https://github.com/DIT112-V20/group-09 # Fetch sources
mkdir build                                # Create build location
cd build
cmake ..                                   # Configure
cmake --build .                            # Build
ctest                                      # Optional: run tests
```

### Developer guidance

[A hacker's guide to SMCE](https://github.com/DIT112-V20/group-09/wiki/A-hacker's-guide-to-SMCE)

### User guidance

[MacOS user guidance](https://github.com/DIT112-V20/group-09/wiki/MacOS-user-build-guidance)

[Windows user guidance](https://github.com/DIT112-V20/group-09/wiki/Windows-user-build-guidance)

### Features overview (not exhaustive)

- Run a basic Arduino sketch in the virtual world
- Support for the Arduino Serial, Wire, Servo, and SPI libraries
- Support for the current Smartcar configuration

[Detailed list](/DIT112-V20/group-09/wiki) under `Milestone details` in the sidebar


[//]: # (##### Will be implemented after v1.0)

[//]: # (- Variable gravity based on height)
[//]: # (- Custom vehicles)

### Non-goals

#### Could be implemented later

- Centrifugal force
- Coriolis force
- Fluids (lift/drag, buoyancy, ...) in the physics simulation
- Android library to communicate with the virtual Bluetooth
- Support for the Arduino WiFi library

#### Will never be part of the project

- Support for another framework than Arduino
