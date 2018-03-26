cmake_minimum_required (VERSION 3.4)

project(rgbled ASM C)

include(build_cortex_m3.cmake)
set(COMMON_FLAGS    "${CORTEX_M3} -Wall -Os -std=gnu11 -fmessage-length=0 -ffunction-sections -fdata-sections -fno-common")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -specs=nosys.specs -Wl,-gc-sections -T ${CMAKE_SOURCE_DIR}/src/LinkerScript.ld -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--print-memory-usage")
set(CMAKE_C_STANDARD_LIBRARIES "-lm -lc -lnosys")

set(COMMON_FLAGS_DEBUG             "-DDEBUG -g3 -flto")
set(CMAKE_ASM_FLAGS_DEBUG          "${COMMON_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG            "${COMMON_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${COMMON_FLAGS_DEBUG}")
set(COMMON_FLAGS_RELEASE           "-DNDEBUG -flto")
set(CMAKE_ASM_FLAGS_RELEASE        "${COMMON_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE          "${COMMON_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")

add_definitions(-DSTM32F1)
add_definitions(-DSTM32F103xB)

file(GLOB incs incs/*)
include_directories(src/appcliation ${incs} incs/MX/MX/Inc)

file(GLOB_RECURSE asmsources FOLLOW_SYMLINKS src/*.s)
file(GLOB_RECURSE csources FOLLOW_SYMLINKS src/*.c)
list(FILTER csources EXCLUDE REGEX ".*_template.c")

add_executable(${PROJECT_NAME}.elf ${csources} ${asmsources})

