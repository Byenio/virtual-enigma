cmake_minimum_required(VERSION 4.0)
project(EnigmaCore VERSION 1.0.0 LANGUAGES C)

include(CTest)

add_library(enigma_core STATIC
        src/rotor.c
        src/reflector.c
        src/plugboard.c
        src/enigma.c
)

target_include_directories(enigma_core PUBLIC include)

add_subdirectory(tests)