macro (cpp_flags_auto arch sanitizers)
    if (NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"
        OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
        message(FATAL_ERROR "unspecialized build type '${CMAKE_BUILD_TYPE}'")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-march=${arch} -flto=thin")
        set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -Og -g3 -glldb -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=${sanitizers}")
        set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -O3 -fwhole-program-vtables")

        if (WIN32)
            string(REPLACE "-fsanitize=${sanitizers}" "-D _DEBUG" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf")
            add_link_options("-Wl,/ignore:longsections")  # dwarf needs long section headers

            if (BUILD_SHARED_LIBS)
                add_compile_definitions("_MT" "_DLL")
                if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
                    link_libraries("msvcrtd" "msvcprtd")
                else ()
                    link_libraries("msvcrt" "msvcprt")
                endif ()
            else ()
                add_compile_definitions("_MT")
                if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
                    link_libraries("libcmtd" "libcpmtd")
                else ()
                    link_libraries("libcmt" "libcpmt")
                endif ()
            endif ()
        else ()
            if (NOT BUILD_SHARED_LIBS)
                add_link_options("-static-libgcc" "-static-libstdc++")
            endif ()
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-march=${arch} -flto")
        set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -Og -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=${sanitizers}")
        set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -O3")

        if (NOT BUILD_SHARED_LIBS)
            add_link_options("-static-libgcc" "-static-libstdc++")
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if ("${arch}" STREQUAL "native" OR "${arch}" STREQUAL "haswell")
            set(arch "AVX2")
        endif ()

        if (BUILD_SHARED_LIBS)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
            set(runtime "MD")
        else ()
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
            set(runtime "MT")
        endif ()

        set(CMAKE_CXX_FLAGS "/nologo /diagnostics:caret /bigobj /MP /EHsc /permissive- /arch:${arch}")
        set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG /${runtime}d /GF /Oi /JMC /RTC1 /ZI")
        set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /${runtime} /O2 /GL /Gw")

        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG:INCREMENTAL")

        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

        set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "")
        set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
    endif ()

    if (CMAKE_C_COMPILER_ID AND NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "${CMAKE_CXX_COMPILER_ID}")
        message(FATAL_ERROR "C/CXX compilers mismatch (C='${CMAKE_C_COMPILER_ID}' CXX='${CMAKE_CXX_COMPILER_ID}')")
    endif ()

    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} $ENV{CFLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")

    if (WIN32)
        link_libraries("onecore.lib")
    else ()
        link_libraries("dl" "pthread" "stdc++fs")
    endif ()

    _set_defaults_flags_cpp_linker()
endmacro ()


function (_set_defaults_flags_cpp_linker)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        string(REGEX MATCH "^[0-9]+" llvm_major "${CMAKE_CXX_COMPILER_VERSION}")

        find_program(lld_PATH NAMES "ld.lld-${llvm_major}" "ld.lld")
        if (lld_PATH)
            message(STATUS "found ld.lld: ${lld_PATH}")
            set(ld_flags "-fuse-ld=lld")
            if (WIN32)
                set(ld_flags "${ld_flags} -Wl,/lldltocache:${CMAKE_BINARY_DIR}/lto_cache")
            else ()
                set(ld_flags "${ld_flags} -Wl,-z,notext -Wl,--thinlto-cache-dir=${CMAKE_BINARY_DIR}/lto_cache")
            endif ()
        else ()
            message(WARNING "could not find lld linker")

            find_program(gold_PATH "ld.gold")
            if (gold_PATH)
                message(STATUS "found ld.gold: ${gold_PATH}")
                set(ld_flags "-fuse-ld=gold")
            else ()
                message(WARNING "could not find gold linker")
            endif ()
        endif ()

        find_program(ar_PATH NAMES "llvm-ar-${llvm_major}" "llvm-ar")
        if (ar_PATH)
            message(STATUS "found llvm-ar: ${ar_PATH}")
            set(CMAKE_AR "${ar_PATH}" PARENT_SCOPE)
        endif ()
        find_program(ranlib_PATH NAMES "llvm-ranlib-${llvm_major}" "llvm-ranlib")
        if (ranlib_PATH)
            message(STATUS "found llvm-ranlib: ${ranlib_PATH}")
            set(CMAKE_RANLIB "${ranlib_PATH}" PARENT_SCOPE)
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        string(REGEX MATCH "^[0-9]+" gcc_major "${CMAKE_CXX_COMPILER_VERSION}")

        find_program(gold_PATH "ld.gold")
        if (gold_PATH)
            message(STATUS "found ld.gold: ${gold_PATH}")
            set(ld_flags "-fuse-ld=gold")
        else ()
            message(WARNING "could not find gold linker")
        endif ()

        find_program(ar_PATH NAMES "gcc-ar-${gcc_major}" "gcc-ar")
        if (ar_PATH)
            message(STATUS "found gcc-ar: ${ar_PATH}")
            set(CMAKE_AR "${ar_PATH}" PARENT_SCOPE)
        endif ()
        find_program(ranlib_PATH NAMES "gcc-ranlib-${gcc_major}" "gcc-ranlib")
        if (ranlib_PATH)
            message(STATUS "found gcc-ranlib: ${ranlib_PATH}")
            set(CMAKE_RANLIB "${ranlib_PATH}" PARENT_SCOPE)
        endif ()
    endif ()

    if (NOT WIN32)
        set(ld_flags "${ld_flags} -Wl,--as-needed")
        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")  # asan rely on this
            set(ld_flags "${ld_flags} -Wl,--no-undefined")
        endif ()
    endif ()

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-s" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ld_flags}" PARENT_SCOPE)
endfunction ()
