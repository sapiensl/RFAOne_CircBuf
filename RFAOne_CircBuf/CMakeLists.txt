# CMakeList.txt : CMake project for RFAOne_CircBuf, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (MAINCODE "main.cpp" "CircularBuffer.h")
add_executable (TESTCODE "tests.cpp" "CircularBuffer.h" "catch.hpp")


enable_testing()
add_test(NAME CIRCBUFTEST COMMAND TESTCODE)