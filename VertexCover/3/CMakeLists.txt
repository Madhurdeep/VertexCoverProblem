cmake_minimum_required(VERSION 2.8)
project(a3ece650)
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

add_executable(a2ece650 a2ece650.cpp )
add_executable(a3ece650 a3ece650.cpp )
add_executable(rgen rgen.cpp )

install(PROGRAMS a1ece650.py DESTINATION bin)
install(TARGETS a2ece650 a3ece650 rgen RUNTIME DESTINATION bin)