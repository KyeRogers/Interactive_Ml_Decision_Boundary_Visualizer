cmake_minimum_required(VERSION 3.15) 
project(ml_boundary_visualizer LANGUAGES C CXX)

# ---- C++ CONFIGURATION ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- FETCH RAYLIB ----
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.5  # Pin to a stable release tag instead of master
)

FetchContent_MakeAvailable(raylib)

# ---- EXECUTABLE CONFIGURATION ----
add_executable(ml_boundary_visualizer src/main.cpp src/point.cpp)

target_include_directories(ml_boundary_visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ml_boundary_visualizer PRIVATE raylib)