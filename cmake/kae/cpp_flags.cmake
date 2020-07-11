macro (cpp_flags_auto)
    if (NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"
        OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
        message(FATAL_ERROR "unspecialized build type '${CMAKE_BUILD_TYPE}'")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-march=haswell")
        set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -Og -g -fno-omit-frame-pointer -fsanitize=address,undefined")
        set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -O3 -flto=thin")

        if (WIN32)
            set(CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG")
            if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
                link_libraries("libcmtd" "libcpmtd")
            else ()
                link_libraries("libcmt" "libcpmt")
            endif ()
        else ()
            add_link_options("-static-libgcc" "-static-libstdc++")
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-march=haswell")
        set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -Og -g -fno-omit-frame-pointer -fsanitize=address,undefined")
        set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -O3 -flto")

        add_link_options("-static-libgcc" "-static-libstdc++")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

        set(CMAKE_CXX_FLAGS "/nologo /diagnostics:caret /bigobj /MP /EHsc /permissive- /arch:AVX2")
        set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG /MTd /GF /Oi /JMC /RTC1 /ZI")
        set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /MT /O2 /GL /Gw")

        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/DEBUG:FULL /LTCG:INCREMENTAL")

        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    
        set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "")
        set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
    endif ()

    if (CMAKE_C_COMPILER_ID)
        if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "${CMAKE_CXX_COMPILER_ID}")
            message(FATAL_ERROR "C/CXX compilers mismatch (C='${CMAKE_C_COMPILER_ID}' CXX='${CMAKE_CXX_COMPILER_ID}')")
        endif ()

        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")

    if (WIN32)
        link_libraries("onecore.lib")
    else ()
        link_libraries("pthread" "stdc++fs")
    endif ()

    _set_defaults_flags_cpp_linker()
endmacro ()


function (_set_defaults_flags_cpp_linker)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        find_program(lld_PATH "ld.lld")
        if (lld_PATH)
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
                set(ld_flags "-fuse-ld=gold")
            else ()
                message(WARNING "could not find gold linker")
            endif ()
        endif ()

        find_program(ar_PATH "llvm-ar")
        if (ar_PATH)
            message(STATUS "found llvm-ar: ${ar_PATH}")
            set(CMAKE_AR "${ar_PATH}" PARENT_SCOPE)
        endif ()
        find_program(ranlib_PATH "llvm-ranlib")
        if (ranlib_PATH)
            message(STATUS "found llvm-ranlib: ${ranlib_PATH}")
            set(CMAKE_RANLIB "${ranlib_PATH}" PARENT_SCOPE)
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        find_program(gold_PATH "ld.gold")
        if (gold_PATH)
            set(ld_flags "-fuse-ld=gold")
        else ()
            message(WARNING "could not find gold linker")
        endif ()
    endif ()

    if (NOT WIN32)
        set(ld_flags "${ld_flags} -Wl,--as-needed,--no-undefined")
    endif ()

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ld_flags}" PARENT_SCOPE)
endfunction ()
