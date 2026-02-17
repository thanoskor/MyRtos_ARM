# Tell CMake it is bare-metal
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Compiler
set(TOOLCHAIN D:/gnu_for_arm/bin)
set(CMAKE_C_COMPILER   ${TOOLCHAIN}/arm-none-eabi-gcc.exe)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}/arm-none-eabi-gcc.exe)
set(CMAKE_LINKER       ${TOOLCHAIN}/arm-none-eabi-ld.exe)

# Flags for bare-metal
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

# Do not link anything by default
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
