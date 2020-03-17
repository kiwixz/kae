function (cpp_test target)
    set(test_target "${target}_test")

    add_executable("${test_target}" "${PROJECT_SOURCE_DIR}/cmake/cpp_test_main.cpp" "${ARGN}")
    target_link_libraries("${test_target}" PRIVATE "${target}" "doctest::doctest")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options("${test_target}" PRIVATE "-fno-sanitize-recover=all")
    endif ()

    add_test(NAME "${test_target}" COMMAND "${test_target}")
endfunction ()
