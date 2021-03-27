# OpenVulkan4D

OpenVulkan4D is in the progress of becoming an open source 4D graphics engine.

## Dependencies

* Vulkan Development Tools

**Arch**
    
    sudo pacman -S vulkan-devel
    
**Windows**

Download and install the latest [1.2.x Vulkan SDK](https://vulkan.lunarg.com/sdk/home).

## Build

**Linux**

    cd OpenVulkan4D/
    mkdir build
    cd build
    cmake ..
    make -j

**Window**

    cd OpenVulkan4D/
    mkdir build
    cd build
    cmake ..
    cmake --build .

## Installation

TBD

## Documentation

**Doxygen**

An automatically generated documentation is being provided through doxygen.

    cd docs
    doxygen
   
The documentation can be accessed by opening the index.html file inside the root documentation directory (docs/).