
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# remove -rdynamic error
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL    ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

SET(CORTEX_M3_CPU_FLAGS "-mthumb -mcpu=cortex-m3 ")
SET(CORTEX_M3_GCC_FLAGS "-ffunction-sections -fdata-sections -fomit-frame-pointer -fno-unroll-loops -ffast-math -ftree-vectorize ")
set(CMAKE_C_FLAGS   " ${CORTEX_M3_CPU_FLAGS} ${CORTEX_M3_GCC_FLAGS}" CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS " ${CORTEX_M3_CPU_FLAGS} ${CORTEX_M3_GCC_FLAGS}" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_ASM_FLAGS " ${CORTEX_M3_CPU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
set(CMAKE_EXE_LINKER_FLAGS    " ${CORTEX_M3_CPU_FLAGS} -Wl,--gc-sections" CACHE INTERNAL "executable linker flags")
set(CMAKE_MODULE_LINKER_FLAGS " ${CORTEX_M3_CPU_FLAGS}" CACHE INTERNAL "module linker flags")
set(CMAKE_SHARED_LINKER_FLAGS " ${CORTEX_M3_CPU_FLAGS}" CACHE INTERNAL "shared linker flags")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

macro(_generate_hex target source)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Obinary "$source" "$target"
    )
endmacro()

macro(_firmware_size target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} -B
        "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}"
    )
endmacro()