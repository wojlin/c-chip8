# CMake minimum version and project name
cmake_minimum_required(VERSION 3.10)
project(CHIP8Emulator C)

# Specify C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add the include directory
include_directories(include)

# Define the source files
set(SOURCES
    src/main.c
    src/chip8.c
    src/chip8_opcodes.c
    src/keyboard.c
    src/display.c
    src/params.c
    src/utils.c
)

# Add the executable
add_executable(chip8 ${SOURCES})

# Link SDL2
target_link_libraries(chip8 ${SDL2_LIBRARIES})


# Optionally, specify compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(chip8 PRIVATE -Wall -Wextra)
endif()
