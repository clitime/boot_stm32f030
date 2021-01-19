cmake_minimum_required(VERSION 3.15)

project(f030_boot C ASM CXX)

message(${CMAKE_C_COMPILER})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(STM32CUBEMX_GENERATED_FILES
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_exti.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_dma.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_gpio.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_i2c.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_crc.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_pwr.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_rcc.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_rtc.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_usart.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_tim.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_utils.c

    src/system/syscalls.c
    src/system/stm32f0xx_it.c
    src/system/system_stm32f0xx.c
    src/system/fullAssert.c
    src/startup/startup_stm32f030c8tx.s

    src/main.cpp
    src/gpio.hpp
    src/usart.hpp src/usart.cpp
    src/timer.cpp
    src/timer.hpp
    src/flashdata.cpp src/flashdata.hpp
    src/configuration.cpp src/configuration.hpp
    src/queue.hpp
    src/system/stm32f0xx_ll_rcc_wrap.h
    src/bootutils.cpp src/bootutils.hpp
    src/protocol/protocol.hpp
    src/applicationprotocol.cpp src/applicationprotocol.hpp)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
    -DUSE_FULL_LL_DRIVER
    -DSTM32F030x8
    )

target_include_directories(${EXECUTABLE} PRIVATE
    src
    src/system
    src/protocol
    vendors/stm32f0xx_hal_driver/Inc
    vendors/cmsis/Include
    vendors/cmsis_core/Include
    )

target_compile_options(${EXECUTABLE} PRIVATE
    -mcpu=cortex-m0
    -mthumb
    -mfloat-abi=soft

    -fdata-sections
    -ffunction-sections

    -Wall
    -Wextra
    -pedantic

    --specs=nano.specs
    -fstack-usage

    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>

    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Release>:-O2>
    )

target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F030C8TX_FLASH.ld
    -mcpu=cortex-m0
    -mthumb
    -mfloat-abi=soft
    -static --specs=nano.specs
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    )

set_source_files_properties(
    src/system/system_stm32f0xx.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_exti.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_rcc.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_utils.c
    vendors/stm32f0xx_hal_driver/Src/stm32f0xx_ll_usart.c
    PROPERTIES
    COMPILE_FLAGS -Wno-unused-parameter)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
