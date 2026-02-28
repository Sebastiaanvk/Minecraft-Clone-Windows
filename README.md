
While in the main folder:
cmake -B build -G "Ninja"
Or for debugging: cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
(Ninja is allegedly a more efficient alternative to make)

ninja: compile the code with the command: ninja -C build

Run the executable with ./build/executable




# Installing dependencies

## glfw
We need glfw.
Download the source from the website. Unzip. Make a build folder. Run  cmake with: "cmake -S . -B build". (I think that works.)
Then do "cmake --build build --config Release" to compile. Then I put the glfw3.lib file(in the build/src/release folder ) in C:/dev/libs/glfw-3.4/lib folder and I put the contents of the include folder in C:/dev/libs/glfw-3.4/include folder. I put both these folders in the cmake file as can be seen in the cmake file.
Also I put the path to this include folder in the vscode c_cpp_properties.json file.

