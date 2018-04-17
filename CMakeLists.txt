cmake_minimum_required (VERSION 3.4)

project(rgbled ASM C)

include(cmake/Platform/cortex-m3.cmake)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32cubemx/STM32F103C8Tx_FLASH.ld)
string(APPEND CMAKE_C_FLAGS  " -std=gnu11 -g3 -Os")
string(APPEND CMAKE_C_CLFAGS " -Wall -Wno-unused-parameter -Wno-nonnull-compare")
string(APPEND CMAKE_C_FLAGS  " -flto -fno-fat-lto-objects")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -specs=nosys.specs \
	-T ${LINKER_SCRIPT} \
	-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map \
	-Wl,--print-memory-usage")
string(APPEND CMAKE_C_STANDARD_LIBRARIES " -lm -lc -lnosys")

string(APPEND CMAKE_C_FLAGS_DEBUG " -DDEBUG")
string(APPEND CMAKE_C_FLAGS_RELEASE "")

add_definitions(-DSTM32F1 -DSTM32F103xB)

include_directories(
	src/application 
	third_party/kamillibc/src/stm32
	third_party/kamillibc/src/lib
	stm32cubemx/Inc
)
file(GLOB_RECURSE src FOLLOW_SYMLINKS
	src/application/*.c
	src/application/*.s
	third_party/kamillibc/src/lib/*.c
	third_party/kamillibc/src/lib/*.s
	third_party/kamillibc/src/stm32/*.c
	third_party/kamillibc/src/stm32/*.s
)

execute_process(
	COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-C_INCLUDES"
	WORKING_DIRECTORY stm32cubemx
	OUTPUT_VARIABLE mx_inc
)
string(REPLACE "-I" "" mx_inc "${mx_inc}")
string(REPLACE " " ";" mx_inc "${mx_inc}")
list(REVERSE mx_inc)
foreach(i ${mx_inc})
	if (${i} MATCHES ".*/STM32Cube_FW_*")
		string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+" "third_party/STM32Cube_FW_F1/" i "${i}")
		string(REGEX REPLACE "\n$" "" i "${i}")
		set(i "${CMAKE_CURRENT_SOURCE_DIR}/${i}")
		if (NOT EXISTS ${i}) 
			MESSAGE(FATAL_ERROR ${i} does not exists)
  		endif()
		include_directories(${i})
	endif()
endforeach()

execute_process(
	COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-C_SOURCES"
	WORKING_DIRECTORY stm32cubemx
	OUTPUT_VARIABLE mx_src
)
string(REPLACE " " ";" mx_src "${mx_src}")
set(tmp "")
foreach(i ${mx_src})
	if (${i} MATCHES ".*/STM32Cube_FW_*")
		string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+/" "third_party/STM32Cube_FW_F1/" i "${i}")
	else()
		string(REGEX REPLACE "Src" "stm32cubemx/Src" i "${i}")
	endif()
	string(REGEX REPLACE "\n$" "" i "${i}")
	set(i "${CMAKE_CURRENT_SOURCE_DIR}/${i}")
	if (NOT EXISTS ${i}) 
		MESSAGE(FATAL_ERROR ${i} does not exists)
  	endif()
	list(APPEND tmp ${i})
endforeach()
set(mx_src "${tmp}")
file(GLOB tmp stm32cubemx/*.s)
list(APPEND mx_src "${tmp}") 

file(GLOB mx_headers stm32cubemx/Inc/*.h)
file(GLOB mx_headers_relative RELATIVE ${CMAKE_SOURCE_DIR}/src/application/machine stm32cubemx/Inc/*.h)
add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h
	COMMAND printf "#include \"%s\"\\n" ${mx_headers_relative} 
		> ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h
	DEPENDS ${mx_headers} COMMENT "Generate mx.h header" VERBATIM 
)
add_custom_target(mx_h DEPENDS ${CMAKE_SOURCE_DIR}/src/application/machine/mx.h)

add_library(mx OBJECT ${mx_src})
target_include_directories(mx PRIVATE stm32cubemx/Inc)
set(mx_objs $<TARGET_OBJECTS:mx>)

add_executable(${PROJECT_NAME}.elf ${src} ${mx_objs})
set_target_properties(${PROJECT_NAME}.elf  PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
add_dependencies(${PROJECT_NAME}.elf mx_h)

