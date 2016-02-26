if(NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "Native compiler detected")
endif()

enable_language(ASM-Z80)

set(CMAKE_C_FLAGS "-mz80 --std-sdcc99 --reserve-regs-iy --max-allocs-per-node 30000")
set(CMAKE_ASM_FLAGS "-p -g")
set(CMAKE_EXE_LINKER_FLAGS "--code-loc 0x9D9B --data-loc 0 --no-std-crt0")

find_package(PythonInterp REQUIRED)
find_program(HEX2BIN hex2bin hex2bin.exe)
if("${HEX2BIN}" STREQUAL "HEX2BIN-NOTFOUND")
    message(FATAL_ERROR "Could not locate hex2bin")
endif()

function(ti_program target calcname)
    add_executable(${target} ${CMAKE_SOURCE_DIR}/c_ti83p/tios_crt0.s ${ARGN})
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${HEX2BIN} ${target}.ihx
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/depends/binpac8x.py -O ${calcname} ${target}.bin ${target}.8xp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Converting to binary"
    )
    set(generated_files
        ${CMAKE_BINARY_DIR}/${target}.8xp
        ${CMAKE_BINARY_DIR}/${target}.bin
        ${CMAKE_BINARY_DIR}/${target}.lk
        ${CMAKE_BINARY_DIR}/${target}.map
        ${CMAKE_BINARY_DIR}/${target}.noi
    )
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${generated_files}")
endfunction()
