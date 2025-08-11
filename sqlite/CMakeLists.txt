cmake_minimum_required(VERSION 4.0)
project(ORM)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Alle Quellcode-Dateien
add_executable(ORM
        orm_types.cpp
        orm_basics.cpp
        orm_table.cpp
        orm.cpp
)

target_link_libraries(ORM sqlite3)
