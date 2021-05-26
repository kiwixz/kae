cmake_minimum_required(VERSION 3.19)

option(KAE_TESTS "Build tests" OFF)

project("kae"
    VERSION 0.0.0
    LANGUAGES CXX
)


file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.cpp")
add_library("kae" ${src})
add_library("kae::kae" ALIAS "kae")
target_include_directories("kae" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries("kae" PUBLIC "fmt::fmt")
if (WIN32)
    target_link_libraries("kae" PUBLIC "Dbghelp")
endif ()


if (KAE_TESTS)
    file(GLOB_RECURSE test_src CONFIGURE_DEPENDS "test/*.cpp")
    add_executable("kae_test" ${test_src})
    target_link_libraries("kae_test" PRIVATE "kae" "doctest::main")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options("kae_test" PRIVATE "-fno-sanitize-recover=all")
    endif ()
    add_test(NAME "kae" COMMAND "kae_test")
endif ()
