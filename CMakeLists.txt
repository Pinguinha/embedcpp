cmake_minimum_required(VERSION 3.14)

project(embedcpp VERSION 0.1 LANGUAGES CXX)

function(embedcpp_generate out_var)
    set(result)
    foreach(in_f ${ARGN})
        file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
        set(out_f "${PROJECT_BINARY_DIR}/${in_f}.embed.hpp")
        add_custom_command(OUTPUT ${out_f}
                COMMAND embedcpp ${src_f} ${out_f}
                DEPENDS ${in_f}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Generating file for embedding ${out_f}"
                VERBATIM)
        list(APPEND result "${out_f}")
    endforeach()
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# The executable code is here
add_subdirectory(app)
