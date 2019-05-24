cmake_minimum_required(VERSION 3.7)
project(ModBusMaster C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MODBUS REQUIRED libmodbus)

add_executable(ModBusMaster main.c)
target_link_libraries(ModBusMaster ${MODBUS_LIBRARIES})
target_include_directories(ModBusMaster PUBLIC ${MODBUS_INCLUDE_DIRS})
target_compile_options(ModBusMaster PUBLIC ${MODBUS_CFLAGS_OTHER})
