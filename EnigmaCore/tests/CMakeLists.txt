include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(enigma_test
        rotor_test.cpp
        reflector_test.cpp
        plugboard_test.cpp
        enigma_test.cpp
)

target_link_libraries(enigma_test
        PRIVATE
        enigma_core
        GTest::gtest_main
)

include(CTest)
add_test(NAME RotorTests COMMAND enigma_test)