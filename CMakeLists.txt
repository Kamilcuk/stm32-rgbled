cmake_minimum_required (VERSION 3.4)

project(rgbled ASM C)

include(cmake/Platform/cortex-m3.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS  " -g3 -O3 -Wall -flto -Wno-unused-parameter")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -specs=nosys.specs \
	-T ${CMAKE_SOURCE_DIR}/stm32cubemx/STM32F103C8Tx_FLASH.ld \
	-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map \
	-Wl,--print-memory-usage")
string(APPEND CMAKE_C_STANDARD_LIBRARIES " -lm -lc -lnosys")

add_definitions(-DSTM32F1 -DSTM32F103xB)

include_directories(
	src/application 
	third_party/kamillibc/src/stm32
	third_party/kamillibc/src/lib
	stm32cubemx/Inc
	third_party/STM32Cube_FW_F1/Drivers/CMSIS/Include
	third_party/STM32Cube_FW_F1/Drivers/CMSIS/Device/ST/STM32F1xx/Include
	third_party/STM32Cube_FW_F1/Drivers/STM32F1xx_HAL_Driver/Inc
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
)
file(GLOB_RECURSE src FOLLOW_SYMLINKS
	src/application/*.c
	src/application/*.s
	third_party/kamillibc/src/lib/*.c
	third_party/kamillibc/src/lib/*.s
	third_party/kamillibc/src/stm32/*.c
	third_party/kamillibc/src/stm32/*.s
)
foreach(i ${src})
	message(STATUS ${i})
endforeach()
file(GLOB_RECURSE hal_src FOLLOW_SYMLINKS
	stm32cubemx/*.s
	stm32cubemx/*.c
	third_party/STM32Cube_FW_F1/Drivers/STM32F1xx_HAL_Driver/*.c
	third_party/STM32Cube_FW_F1/Drivers/STM32F1xx_HAL_Driver/*.s
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/*.c
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/*.s
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Core/*.c
	third_party/STM32Cube_FW_F1/Middlewares/ST/STM32_USB_Device_Library/Core/*.s
)
list(FILTER hal_src EXCLUDE REGEX ".*_template.c")

file(GLOB mx_headers stm32cubemx/Inc/*.h)
file(GLOB mx_headers_relative RELATIVE ${CMAKE_SOURCE_DIR}/src/application/machine stm32cubemx/Inc/*.h)
add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h
	COMMAND printf "#include \"%s\"\\n" ${mx_headers_relative} 
		> ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h
	DEPENDS ${mx_headers} COMMENT "Generate mx.h header" VERBATIM 
)
add_custom_target(mx_h DEPENDS ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h)

add_library(hal OBJECT ${hal_src})
target_include_directories(hal PRIVATE stm32cubemx/Inc)
set(hal_objs $<TARGET_OBJECTS:hal>)

add_executable(${PROJECT_NAME}.elf ${src} ${hal_objs})
add_dependencies(${PROJECT_NAME}.elf mx_h)

