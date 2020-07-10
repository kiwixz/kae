include(ExternalProject)

function (download name url)
    set(dest "${CMAKE_CURRENT_BINARY_DIR}/${name}")
    if (NOT EXISTS "${dest}")
        message(STATUS "downloading ${name}")
        file(DOWNLOAD "${url}" "${dest}")
    endif ()
    set(${name}_PATH "${dest}" PARENT_SCOPE)
endfunction ()


function (download_github name repo ref file)
    download("${name}" "https://raw.githubusercontent.com/${repo}/${ref}/${file}")
    set(${name}_PATH "${${name}_PATH}" PARENT_SCOPE)
endfunction ()
