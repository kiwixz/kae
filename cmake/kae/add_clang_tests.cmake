set(kae_cmake_dir "${CMAKE_CURRENT_LIST_DIR}")

function (add_clang_tests dir)
    find_package("Python3" REQUIRED "Interpreter")
    add_test(NAME "clang_format"
        COMMAND "${Python3_EXECUTABLE}" "-OO" "-u" "${kae_cmake_dir}/check_clang_format.py" "${dir}"
    )
    add_test(NAME "clang_tidy"
        COMMAND "${Python3_EXECUTABLE}" "-OO" "-u" "${kae_cmake_dir}/check_clang_tidy.py" "${dir}"
    )
endfunction ()
