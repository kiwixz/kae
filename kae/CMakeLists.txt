cmake_minimum_required(VERSION 3.12)
include("kae/cpp_test")

set(target "kae")

file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.cpp")
add_library("${target}" ${src})
add_library("${PROJECT_NAME}::${target}" ALIAS "${target}")
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (KAE_TESTS)
    file(GLOB_RECURSE test_src CONFIGURE_DEPENDS "test/*.cpp")
    cpp_test("${target}" ${test_src})
endif ()


target_link_libraries("${target}" PUBLIC
    "fmt::fmt"
)

if (WIN32)
    target_link_libraries("${target}" PUBLIC
        "Dbghelp"
    )
endif ()
