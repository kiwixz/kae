cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("kae/add_clang_tests")
include("kae/cpp_compiler")
include("kae/cpp_flags")
include("kae/cpp_warnings")
include("kae/set_default")


if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_ROOT_LIST ON)
else ()
    set(IS_ROOT_LIST OFF)
endif ()


option(KAE_TESTS "Build tests" ${IS_ROOT_LIST})
option(KAE_AUTO_COMPILERS "Automatically override compilers" ${IS_ROOT_LIST})
option(KAE_AUTO_FLAGS "Automatically override flags" ${IS_ROOT_LIST})
set(KAE_ARCH "native" CACHE STRING "Machine architecture")

if (IS_ROOT_LIST)
    set_default(CMAKE_BUILD_TYPE "Debug" STRING)
    set_default(CMAKE_MESSAGE_CONTEXT_SHOW ON BOOL)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif ()


if (KAE_AUTO_COMPILERS)
    cpp_compiler_auto()
endif ()

project("kae"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

if (KAE_AUTO_FLAGS)
    cpp_flags_auto("${KAE_ARCH}")
    cpp_warnings_auto()
endif ()

if (KAE_TESTS)
    enable_testing()
    if (IS_ROOT_LIST)
        add_clang_tests("${PROJECT_SOURCE_DIR}")
    endif ()
endif ()

file(GLOB_RECURSE data RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/data" CONFIGURE_DEPENDS "data/*")
foreach (file IN LISTS data)
    add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/data/${file}"
        COMMAND cmake -E copy "${CMAKE_CURRENT_SOURCE_DIR}/data/${file}" "${CMAKE_BINARY_DIR}/data/"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/${file}"
        COMMENT "Copying data/${file}..."
        VERBATIM
    )
    add_custom_target("data_${file}" ALL DEPENDS "${CMAKE_BINARY_DIR}/data/${file}")
endforeach ()


add_subdirectory("third_party/doctest")
add_subdirectory("third_party/fmt")

add_subdirectory("kae")
