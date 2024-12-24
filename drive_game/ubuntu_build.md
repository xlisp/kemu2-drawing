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

