cmake_minimum_required(VERSION 3.20)

project(rtos C ASM)

set(CMAKE_C_STANDARD 11)

# CPU/FPU flags
set(MCPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${MCPU_FLAGS} -ffreestanding")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "${MCPU_FLAGS} -nostdlib")

add_executable(rtos.elf
    startup.c
    main.c
    uart.c
)

target_link_libraries(rtos.elf gcc)

target_link_options(rtos.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker.ld
    -Wl,--gc-sections
)
