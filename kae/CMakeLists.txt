cmake_minimum_required(VERSION 3.12)
include("test_cpp")

set(target "kae")

file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.cpp")
add_library("${target}" STATIC ${src})
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (KAE_TESTS)
    file(GLOB_RECURSE test_src CONFIGURE_DEPENDS "test/*.cpp")
    test_cpp("${target}" ${test_src})
endif ()


target_link_libraries("${target}"
    "fmt::fmt"
)

if (WIN32)
    target_link_libraries("${target}"
        "Dbghelp"
    )
endif ()
