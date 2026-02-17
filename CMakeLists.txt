cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

project(rtos C ASM)

# Compiler settings
set(CMAKE_C_COMPILER D:/gnu_for_arm/bin/arm-none-eabi-gcc.exe)
set(CMAKE_ASM_COMPILER D:/gnu_for_arm/bin/arm-none-eabi-gcc.exe)

# Ensure ASM sees same flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

# Build type
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_STANDARD 11)

# Define executable
add_executable(rtos.elf
        startup.c
        main.c
)

# Linker script and options
target_link_options(rtos.elf PRIVATE
        -T${CMAKE_SOURCE_DIR}/linker.ld
        -Wl,--gc-sections
)
