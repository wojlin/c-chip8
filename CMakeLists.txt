# CMake minimum version and project name
cmake_minimum_required(VERSION 3.10)
project(CHIP8Emulator C)

# Specify C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the include directory
include_directories(include)

# Define the source files
set(SOURCES
    src/main.c
    src/chip8.c
)

# Add the executable
add_executable(chip8_emulator ${SOURCES})

# Optionally, specify compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(chip8_emulator PRIVATE -Wall -Wextra)
endif()
