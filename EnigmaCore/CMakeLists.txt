cmake_minimum_required(VERSION 4.0)
project(EnigmaCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_library(enigma_core STATIC
        src/rotor.cpp
        src/reflector.cpp
        src/plugboard.cpp
        src/enigma.cpp
)

target_include_directories(enigma_core PUBLIC include)

add_subdirectory(tests)