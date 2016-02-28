cmake_minimum_required(VERSION 2.8)

project(ticg)
set(CMAKE_BUILD_TYPE Release)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source build detected")
endif()

set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake/Modules"
    "${CMAKE_MODULE_PATH}"
)

include(TIProgram)

ti_program(snake snake snake.c apple.c draw.asm c_ti83p/src/output.asm c_ti83p/src/iongraphics.asm)
