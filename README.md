# embedcpp
Easy embed of resource files to C++ source code, for CMake and C++17.

During build, the content of the selected resource files will be made available in `std::string_view` objects inside auto-generated header files. Each object is `inline constexpr`, making it so that there will only be one definition over all translation units that may include it. CMake will also only run the executable for generating the files during build if the resource file is newer than the generated file, or if the generated file does not exist.

Include this project in your CMake project, with the `FetchContent` module, as shown below, for example, or some other method, such as git submodules.

    include(FetchContent)
    FetchContent_Declare(
                        embedcpp
                        GIT_REPOSITORY https://github.com/Pinguinha/embedcpp.git
                        GIT_TAG {PICK A TAG}
    )
    FetchContent_MakeAvailable(embedcpp)

Call the `embedcpp_generate()` function in your `CMakeLists.txt`. For example:

    embedcpp_generate(GENERATED_FILEPATHS INC_DIR shaders/vertex.glsl shaders/fragment.glsl)

The function takes at least 3 arguments. The first is an output variable that will be filled with the complete path to each of the generated files. The second is an output variable that will be filled with the include directory. The third and so on are the resource files. They can be on relative path to the `CMakeLists.txt` that will be calling the function.

Complete your `CMakeLists.txt` by updating your list of header files and include dirs. For example:

    add_library(my_lib ${SOURCE_LIST} ${HEADER_LIST} ${GENERATED_FILEPATHS})
    target_include_directories(my_lib PUBLIC ../include ${INC_DIR})

The generated files will be located in the build folder. They can now be included in your source files.

    #include <embedcpp/shaders/vertex.glsl.embed.hpp>

And the resource file content will be available via `std::string_view` interface.

    std::cout << embedcpp::vertex_glsl_view << std::endl;
    std::cout << embedcpp::fragment_glsl_view << std::endl;

### Credits

Inspired by:
https://stackoverflow.com/a/11814544
