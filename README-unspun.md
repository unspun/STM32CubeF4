# Changes from mainline
1. Add CMake build support

# CMake build support
## How to use lib
1. (optional) Set the vector table offset. This is used to build the library for an application that doesn't start at the beginning of flash. 
Example:
```
set(STM32F4XX_HAL_VECTOR_OFFSET 0x20000)
```
2. Call `unspun_add_stm32f4_hal` with the desired target name and the location of the configuration file. 
Example:
```
unspun_add_stm32f4_hal(stm32f413xx_bsp, ${CMAKE_CURRENT_SOURCE_DIR}/inc)
```

Also see the test_cmake folder for working example. 

## Supported chipsets
- STM32F413xx

## Run the CMake build tests
### Pre-Reqs
1. ARM GCC tool chain is installed and has been added to the PATH
2. Make is installed and has been added to the PATH
### Build
1. Configure build (first time only)
```
cmake -S test_cmake -B build -G "Unix Makefiles" --toolchain toolchain.cmake
```
2. Run the build command:
```
cmake --build build
```
This will create a folder called "build" in the root of the repo that will contain all the built artifacts