cmake_minimum_required(VERSION 3.16)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

#set(MCU "atmega328p")
#set(F_CPU "16000000")

set(MCU "attiny85")
set(F_CPU "16500000")

set(CMAKE_C_COMPILER /usr/local/bin/avr-gcc CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER /usr/local/bin/avr-g++ CACHE STRING "C++ compiler" FORCE)
set(CMAKE_OSX_SYSROOT "")

set(C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -fdata-sections -ffunction-sections -Wl,--gc-sections")
set(LD_FLAGS "-mmcu=${MCU}")

set(CMAKE_C_FLAGS "${C_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${LD_FLAGS}")
set(CMAKE_CXX_FLAGS "${C_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "${LD_FLAGS}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Building as MinSizeRel")
    set(CMAKE_BUILD_TYPE "MinSizeRel")
endif ()

project(ir_hid)
set_property(SOURCE src/usbdrv/usbdrvasm.S PROPERTY LANGUAGE CXX)
add_executable(ir_hid
        src/main.c
        src/ir.cpp
#        src/uart.c
        src/usb.c
        src/usbdrv/usbdrvasm.S
        src/usbdrv/usbdrv.c)
# TODO: Create separate project for irmp
target_include_directories(ir_hid PRIVATE
        externals/irmp/src
        src/
        src/usbdrv)

add_custom_command(TARGET ir_hid
        POST_BUILD
        COMMAND avr-objcopy -j .text -j .data -O ihex $<TARGET_FILE:ir_hid> ir_hid.hex
        COMMAND avr-objdump -CS $<TARGET_FILE:ir_hid> > ir_hid.S
        COMMAND avr-size -C --mcu=${MCU} $<TARGET_FILE:ir_hid>
        )