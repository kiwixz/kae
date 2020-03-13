cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("set_defaults")


option(KAE_TESTS "Build tests" ON)


set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS}" CACHE STRING "")
set_defaults_tools_cpp()

project("kae"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_defaults_flags_cpp()

if (KAE_TESTS)
    enable_testing()
    find_package("Python3" REQUIRED "Interpreter")
    add_test(NAME "check_clang_format"
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/check_clang_format.py"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)  # only do this check if we are the root project
        add_test(NAME "check_clang_tidy"
            COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/check_clang_tidy.py" "${CMAKE_BINARY_DIR}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        )
    endif ()
endif ()


add_subdirectory("third_party/doctest")
add_subdirectory("third_party/fmt")

add_subdirectory("${PROJECT_NAME}")
