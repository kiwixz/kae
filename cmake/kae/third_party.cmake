include(FetchContent)

function (third_party name)
    cmake_parse_arguments(ARG "DONT_RUN_CMAKE" "REPO;TAG;URL" "PATCHES" ${ARGN})

    if (ARG_REPO AND ARG_TAG AND NOT ARG_URL)
        FetchContent_Declare("${name}"
            GIT_REPOSITORY "https://github.com/${ARG_REPO}"
            GIT_TAG "${ARG_TAG}"
        )
    elseif (ARG_URL AND NOT ARG_REPO AND NOT ARG_TAG)
        FetchContent_Declare("${name}" URL "${ARG_URL}")
    else ()
        message(FATAL_ERROR "wrong arguments")
    endif()

    FetchContent_GetProperties("${name}")
    if (${name}_POPULATED)
        message(STATUS "third party '${name}' already populated, skipping")
        return()
    endif()

    message(STATUS "fetching ${name}")
    FetchContent_Populate("${name}")

    if (ARG_PATCHES)
        message(STATUS "patching ${name}")
        _apply_patches("${${name}_SOURCE_DIR}" PATCHES ${ARG_PATCHES})
    endif ()

    if (NOT ARG_DONT_RUN_CMAKE)
        message(STATUS "adding ${name}")
        suppress_warnings_cpp()
        list(APPEND CMAKE_MESSAGE_CONTEXT "${name}")
        add_subdirectory("${${name}_SOURCE_DIR}" "${${name}_BINARY_DIR}" EXCLUDE_FROM_ALL)
        list(POP_BACK CMAKE_MESSAGE_CONTEXT)
    endif ()
endfunction ()


function (system_include_dirs target)
    get_target_property(incs "${target}" INTERFACE_INCLUDE_DIRECTORIES)
    set_target_properties("${target}" PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${incs}"
    )
endfunction ()


function (suppress_warnings_cpp)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" PARENT_SCOPE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w" PARENT_SCOPE)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        string(REGEX REPLACE "(^| )[/-]W([0-4]|all)" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0" PARENT_SCOPE)

        string(REGEX REPLACE "(^| )[/-]W([0-4]|all)" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0" PARENT_SCOPE)
    endif ()

    if (WIN32)
        add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
    endif ()
endfunction ()


function (_apply_patches dir)
    cmake_parse_arguments(ARG "" "" "PATCHES" ${ARGN})

    find_program(git_PATH git)
    foreach (patch ${ARG_PATCHES})
        get_filename_component(abs_patch "${patch}" ABSOLUTE)

        execute_process(COMMAND "${git_PATH}" apply --ignore-whitespace --check --reverse "${abs_patch}"
            WORKING_DIRECTORY "${dir}"
            RESULT_VARIABLE exit_code
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if (exit_code)
            message(STATUS "applying patch '${patch}'")
            execute_process(COMMAND "${git_PATH}" apply --ignore-whitespace "${abs_patch}"
                WORKING_DIRECTORY "${dir}"
                RESULT_VARIABLE exit_code
            )
            if (exit_code)
                message(FATAL_ERROR "could not apply patch '${patch}'")
            endif ()
        else ()
            message(STATUS "patch '${patch}' already applied")
        endif ()
    endforeach ()
endfunction ()
