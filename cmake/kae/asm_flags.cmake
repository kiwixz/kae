macro (asm_flags_init)
    set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Wall")
    set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g")
    set(CMAKE_ASM_NASM_FLAGS_RELEASE "")

    if (WIN32)
        set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Xvc")
    endif ()
endmacro ()
