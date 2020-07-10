cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("kae/cpp_compiler")
include("kae/cpp_flags")
include("kae/cpp_warnings")


if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_ROOT_LIST ON)
else ()
    set(IS_ROOT_LIST OFF)
endif ()


option(KAE_TESTS "Build tests" ${IS_ROOT_LIST})
option(KAE_AUTO_COMPILERS "Automatically override compilers" ${IS_ROOT_LIST})
option(KAE_AUTO_FLAGS "Automatically override flags" ${IS_ROOT_LIST})


if (IS_ROOT_LIST AND NOT CMAKE_BUILD_TYPE)
    message(STATUS "defaulting CMAKE_BUILD_TYPE to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
endif ()
if (IS_ROOT_LIST AND NOT CMAKE_MESSAGE_CONTEXT_SHOW)
    message(STATUS "defaulting CMAKE_MESSAGE_CONTEXT_SHOW to ON")
    set(CMAKE_MESSAGE_CONTEXT_SHOW ON CACHE BOOL "")
endif ()

if (KAE_AUTO_COMPILERS)
    cpp_compiler_auto()
endif ()

project("kae"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (KAE_AUTO_FLAGS)
    cpp_flags_auto()
    cpp_warnings_auto()
endif ()

if (KAE_TESTS)
    enable_testing()
    if (IS_ROOT_LIST)
        find_package("Python3" REQUIRED "Interpreter")
        add_test(NAME "check_clang_format"
            COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/check_clang_format.py"
        )
        add_test(NAME "check_clang_tidy"
            COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/check_clang_tidy.py" "${CMAKE_BINARY_DIR}"
        )
    endif ()
endif ()


add_subdirectory("third_party/doctest")
add_subdirectory("third_party/fmt")

add_subdirectory("kae")
