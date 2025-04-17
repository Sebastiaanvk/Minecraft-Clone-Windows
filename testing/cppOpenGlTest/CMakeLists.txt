cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_ROOT "C:/dev/libs/glfw-3.4")

# Add the GLFW include directory
include_directories(${GLFW_ROOT}/include include)

# Add the GLFW library directory
link_directories(${GLFW_ROOT}/lib)

# add_definitions(-DGLAD_GLAPI_EXPORT)

file(GLOB SOURCES "src/*.c" "src/*.cpp"  )
add_executable(executable ${SOURCES})

target_link_libraries(executable glfw3 opengl32 )