# Run

```
(base) ➜  driving_game git:(main) ✗ ./run.sh 
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
CMake Warning (dev) at /usr/share/cmake-3.28/Modules/FindOpenGL.cmake:381 (message):
  Policy CMP0072 is not set: FindOpenGL prefers GLVND by default when
  available.  Run "cmake --help-policy CMP0072" for policy details.  Use the
  cmake_policy command to set the policy and suppress this warning.

  FindOpenGL found both a legacy GL library:

    OPENGL_gl_LIBRARY: /usr/lib/x86_64-linux-gnu/libGL.so

  and GLVND libraries for OpenGL and GLX:

    OPENGL_opengl_LIBRARY: /usr/lib/x86_64-linux-gnu/libOpenGL.so
    OPENGL_glx_LIBRARY: /usr/lib/x86_64-linux-gnu/libGLX.so

  OpenGL_GL_PREFERENCE has not been set to "GLVND" or "LEGACY", so for
  compatibility with CMake 3.10 and below the legacy GL library will be used.
Call Stack (most recent call first):
  CMakeLists.txt:5 (find_package)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Found OpenGL: /usr/lib/x86_64-linux-gnu/libOpenGL.so   
-- Found GLUT: /usr/lib/x86_64-linux-gnu/libglut.so  
-- Configuring done (0.6s)
-- Generating done (0.0s)
-- Build files have been written to: /home/xlisp/CppPro/kemu2-drawing/driving_game/build
[ 50%] Building CXX object CMakeFiles/driving_game.dir/driving_game.cpp.o
[100%] Linking CXX executable driving_game
[100%] Built target driving_game
Game Over!
Congratulations! Final score: 100

```

I'll help you set up and run the game on Ubuntu. Here are the step-by-step instructions:

1. First, install the required development packages:
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev
sudo apt-get install cmake
```

2. Save the game code in a file named `driving_game.cpp`

3. Create a CMakeLists.txt file:



```cmake
cmake_minimum_required(VERSION 3.10)
project(DrivingGame)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Add executable
add_executable(driving_game driving_game.cpp)

# Link libraries
target_link_libraries(driving_game
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    GL
    GLU
    glut
)

# Include directories
target_include_directories(driving_game PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

# Set C++ standard
set_target_properties(driving_game PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

```

4. Create a directory structure and compile:
```bash
# Create a new directory for your game
mkdir driving_game
cd driving_game

# Copy your driving_game.cpp and CMakeLists.txt into this directory

# Create and enter build directory
mkdir build
cd build

# Generate build files and compile
cmake ..
make
```

5. Run the game:
```bash
./driving_game
```

Game Controls:
- W: Move forward
- S: Move backward
- A: Turn left
- D: Turn right
- Space: Stop
- R: Reset game

If you encounter any errors like "GL/glut.h: No such file or directory", make sure all dependencies are properly installed by running:
```bash
sudo apt-get install mesa-common-dev
sudo apt-get install libgl1-mesa-dev
```

Let me know if you run into any issues while compiling or running the game!

