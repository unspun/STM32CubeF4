# Changes from mainline
1. Add CMake build support

# CMake build support
## How to use lib
1. Set the config path using the STM32F4XX_HAL_CONF_FILE variable.
Example:
```
set(STM32F4XX_HAL_CONF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/inc/stm32f4xx_hal_conf.h)
```
2. (optional) Set the vector table offset. This is used to build the library for an application that doesn't start at the beginning of flash. 
Example:
```
set(STM32F4XX_HAL_VECTOR_OFFSET 0x20000)
```
3. Add the subdirectory and set the binary path (second argument in add_subdirectory). The binary name will be used to name the target, so subdirectory can be added multiple times with different configurations.
Example:
```
add_subdirectory(../ stm32f413xx_bsp)
```
4. Add directory containing the HAL configuration to the library build. Example:
```
target_include_directories(stm32f413xx_bsp
    PUBLIC
    inc/
)
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