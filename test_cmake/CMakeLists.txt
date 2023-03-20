project(stm32f413xx_build_test)

add_subdirectory(../ stm32f4)

set (hal_sources 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_conf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_desc.c
)

set (hal_includes 
  ${unspun_stm32f4_platform_internal_dir}/inc
)

# Example standard build target
unspun_add_stm32f4_hal(stm32f413xx_bsp 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc 
  "${hal_board_sources}" 
  "${hal_includes}"
  ENABLE_USB_DEVICE_CDC)

# Example vector table offset build target
set(STM32F4XX_HAL_VECTOR_OFFSET 0x20000)
unspun_add_stm32f4_hal(stm32f413xx_bsp_offset ${CMAKE_CURRENT_SOURCE_DIR}/inc "" "")