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
