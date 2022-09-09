project(stm32f413xx_build_test)

set(STM32F4XX_HAL_CONF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/inc/stm32f4xx_hal_conf.h)
message(STATUS "Set STM32F4XX_HAL_CONF_FILE=${STM32F4XX_HAL_CONF_FILE}")

# Example standard build target
add_subdirectory(../ stm32f413xx_bsp)
target_include_directories(stm32f413xx_bsp
    PRIVATE
    inc/
)
add_library(test_build
    STATIC
    inc/stm32f4xx_hal_conf.h
)
    
target_link_libraries(test_build
    stm32f413xx_bsp
)

# Example vector table offset build target
set(STM32F4XX_HAL_VECTOR_OFFSET 0x20000)
add_subdirectory(../ stm32f413xx_bsp_offset)
target_include_directories(stm32f413xx_bsp_offset
    PRIVATE
    inc/
)
add_library(offset_test_build
    STATIC
    inc/stm32f4xx_hal_conf.h
)
target_link_libraries(offset_test_build
    stm32f413xx_bsp_offset
)
