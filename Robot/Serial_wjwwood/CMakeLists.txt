cmake_minimum_required(VERSION 2.8.3)
project(my_project)

include_directories(include)

set(serial_SRCS src/serial.cc include/serial/serial.h include/serial/v8stdint.h)
list(APPEND serial_SRCS src/impl/unix.cc)

## Add serial library
add_library(serial ${serial_SRCS})
target_link_libraries(serial rt)