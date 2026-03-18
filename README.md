# Minecraft Clone in C++ and OpenGL
<img src="assets\media\demo shorter 18-3-2026.gif" width="1000" >

<img src="assets\media\picture 18-3-2026.png" width="700" ><img src="assets\media\underwater 18-3-2026.png" width="700" >

This is my project where I try to make a clone of Minecraft.
The code is in C++ and I use OpenGL and GLFW to render triangles using the GPU.
I use no framework or engine and I try to use external libraries as little as possible.

I started this project, because I have always been really interested in game development and graphics and I love Minecraft. 
A voxel engine is also a perfect sandbox for learning about graphics and relevant mathematics.
I have a lot of experience with algorithm puzzles and scientific programming, but not really with a big software engineering project like this.

My goal is to learn and to practice relevant skills, so I also try minimize AI dependence.
I do use it for looking up syntax and documentation, asking about conventional patterns for, for example, the architecture and the occasional debugging.
Pretty much I treat it as a Teaching Assistant at university.
99% of the lines are written by me.
I've also experienced that as of 2026, the free LLM's are pretty useless when it comes to the mathematics behind graphics rendering.
They know the relevant terms, but they are absolutely bluffing when it comes to the intricacies.

I spent a lot of time optimizing the code for world generation and mesh calculation.
These are both done asynchronously.
I'm pretty happy with how smooth generating new chunks currently is.

# DevLog
[Check out the DevLog!](devLog.md)

I started the devlog in March 2026, but at that point I already spent a lot of time learning about OpenGl, setting up the architecture, getting chunk rendering working, optimizing the calculation of the chunk meshes, etc.


# Compiling and running the executable:
While in the main folder:
cmake -B build -G "Ninja"
Or for debugging: cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
(Ninja is allegedly a more efficient alternative to make)

ninja: compile the code with the command: ninja -C build

Run the executable with ./build/executable




# Installing dependencies

## cpp compiler
Most of yall will know this, but Im a dumbass who forgets stuff, so this is mostly for myself.
I use the gcc compiler from mingw, installed using msys2.
So download MSYS2 (to folder C:/msys64 ) and then in the msys2 window run: pacman -S mingw-w64-x86_64-gcc.
This should fill up the binary folder somewhere in like C:/msys64/mingw64/bin.
In there, you will find an executable called gcc.exe.

Now we also need to add the paths to the C and CPP compilers to the Windows Environment variables.
For this, we go to the "Edit the system environment variables" and then find Path and edit it and add "C:\msys64\mingw64\bin" or wherever you installed the binaries.
You can do the path in User Variables, unless you want the path for all users.
In that case, add it to the path in System Variables.

Put the proper compiler path in c_cpp_properties.json for linting.

## Ninja
This is just a faster version of make.
I installed it by running "winget install Ninja-build.Ninja" in the terminal.


## GLFW
We need glfw to use OpenGL.

IMPORTANT: Make sure you use the same compiler for compiling your project as the GLFW library!
You can also find pre-compiled binaries on the GLFW website.
Download the pre-compiled binaries and since Im using the mingw compilers, I used the contents of lib-mingw-w64 folder.

Alternatively, download the source from the website. Unzip. Run  cmake with: "cmake -S . -B build". (I think that works.)
Then do "cmake --build build --config Release" to compile. (like I said, make sure youre using the same compiler as the project).

Then I put the glfw3.lib file (in the build/src/release folder ) in a newly made C:/dev/libs/glfw-3.4/lib folder and I put the contents of the include folder in a newly made C:/dev/libs/glfw-3.4/include folder.
I put a path to both these folders in the cmake file as can be seen in the cmake file. 
Also I put the path to this include folder in the vscode c_cpp_properties.json file.

## Debugger
I use the mingw debugger.
To install type: "pacman -S mingw-w64-x86_64-gdb" in MSYS2.


