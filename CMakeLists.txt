cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico-mcp9808)

pico_sdk_init()

add_executable(mcp9808
    mcp9808.c
)

target_link_libraries(mcp9808
    pico_stdlib
    hardware_i2c
)

pico_enable_stdio_usb(mcp9808 1)
pico_enable_stdio_uart(mcp9808 0)

pico_add_extra_outputs(mcp9808)
