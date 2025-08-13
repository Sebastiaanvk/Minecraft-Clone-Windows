Navigate to build folder.

Cmake: run the cmake with: cmake .. -G "Ninja"
Or for debugging:  cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug

After moving back to the main folder.
ninja: compile the code with the command ninja -C build

Run the executable with ./build/executable




# Installing dependencies

## glfw
We need glfw.
Download the source from the website. Unzip. Make a build folder. Run  cmake with: "cmake -S . -B build". (I think that works.)
Then do "cmake --build build --config Release" to compile. Then I put the glfw3.lib file(in the build/src/release folder ) in C:/dev/libs/glfw-3.4/lib folder and I put the contents of the include folder in C:/dev/libs/glfw-3.4/include folder. I put both these folders in the cmake file as can be seen in the cmake file.
Also I put the path to this include folder in the vscode c_cpp_properties.json file.


# Cmake file
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_ROOT "C:/dev/libs/glfw-3.4")

'#' Add the GLFW include directory
include_directories(${GLFW_ROOT}/include include)

'#' Add the GLFW library directory
link_directories(${GLFW_ROOT}/lib)

'#' add_definitions(-DGLAD_GLAPI_EXPORT)

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/external/*.cpp" "src/external/*.c" "src/input/*.cpp" "src/input/*.c" "src/render/*.cpp" "src/render/*.c" "src/world/*.cpp" "src/world/*.c" "src/shaders/*.cpp")
add_executable(executable ${SOURCES})


target_link_libraries(executable glfw3 opengl32)


