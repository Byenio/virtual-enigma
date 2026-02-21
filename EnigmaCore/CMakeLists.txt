cmake_minimum_required(VERSION 3.20)
project(EnigmaCore VERSION 1.0.0 LANGUAGES C CXX)

include(CTest)

add_library(enigma_core STATIC
        src/rotor.c
        src/reflector.c
        src/plugboard.c
        src/enigma.c
        src/constants.c
)

target_include_directories(enigma_core PUBLIC include)

add_subdirectory(tests)