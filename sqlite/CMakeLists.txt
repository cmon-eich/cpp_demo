cmake_minimum_required(VERSION 4.0)
project(Sqlite)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Alle Quellcode-Dateien
add_executable(Sqlite
        sqlite.cpp
)

target_link_libraries(Sqlite sqlite3)
