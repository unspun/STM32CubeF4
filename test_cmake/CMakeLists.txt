project(stm32f413xx_build_test)

add_subdirectory(../ stm32f4)

# Example standard build target
unspun_add_stm32f4_hal(stm32f413xx_bsp, ${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_library(test_build
    STATIC
    inc/stm32f4xx_hal_conf.h
)

target_link_libraries(test_build
    stm32f413xx_bsp
)

# Example vector table offset build target
set(STM32F4XX_HAL_VECTOR_OFFSET 0x20000)
unspun_add_stm32f4_hal(stm32f413xx_bsp_offset, ${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_library(offset_test_build
    STATIC
    inc/stm32f4xx_hal_conf.h
)
target_link_libraries(offset_test_build
    stm32f413xx_bsp_offset
)
