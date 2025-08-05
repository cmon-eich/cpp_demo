cmake_minimum_required(VERSION 4.0)
project(Calculator)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Alle Quellcode-Dateien
add_executable(Calculator
        main.cpp
        tokenizer.cpp
)
