function (cpp_warnings_auto)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(flags
            "-Weverything"

            "-Wno-c++98-compat-pedantic"
            "-Wno-covered-switch-default"
            "-Wno-ctad-maybe-unsupported"
            "-Wno-double-promotion"
            "-Wno-exit-time-destructors"
            "-Wno-float-equal"
            "-Wno-padded"
            "-Wno-sign-conversion"
            "-Wno-weak-vtables"
        )
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(flags "-pedantic" "-Wall" "-Wextra")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(flags
            "/Wall"

            "/wd4365"
            "/wd4514"
            "/wd4571"
            "/wd4625"
            "/wd4626"
            "/wd4710"
            "/wd4711"
            "/wd4800"
            "/wd4820"
            "/wd5026"
            "/wd5027"
            "/wd5045"

            # false positives
            "/wd4355"
            "/wd4868"
            "/wd5204"

            "/experimental:external"
            "/external:W0"
            "/external:env:INCLUDE"
        )
        set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I" PARENT_SCOPE)
        set(CMAKE_INCLUDE_SYSTEM_FLAG_C "/external:I" PARENT_SCOPE)
    endif ()

    list(JOIN flags " " flags_str)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_str}" PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags_str}" PARENT_SCOPE)
endfunction ()
