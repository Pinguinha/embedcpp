cmake_minimum_required(VERSION 3.14)

project(embedcpp VERSION 0.2 LANGUAGES CXX)

function(embedcpp_generate path_to_gen_files include_dir)
    set(result)
    foreach(input_file ${ARGN})
        file(RELATIVE_PATH input_file_rel_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${input_file})
        set(dest_path "${PROJECT_BINARY_DIR}/embedcpp/embedcpp/${input_file}.embed.hpp")
        add_custom_command(OUTPUT ${dest_path}
                COMMAND embedcpp ${input_file_rel_path} ${dest_path}
                DEPENDS ${input_file}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Generating file for embedding ${dest_path}"
                VERBATIM)
        list(APPEND result "${dest_path}")
    endforeach()
    set(${path_to_gen_files} "${result}" PARENT_SCOPE)
    set(${include_dir} "${PROJECT_BINARY_DIR}/embedcpp/" PARENT_SCOPE)
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
