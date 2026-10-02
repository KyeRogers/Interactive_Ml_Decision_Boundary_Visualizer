cmake_minimum_required(VERSION 3.15) 
project(ml_boundary_visualizer LANGUAGES CXX)

# ---- CONFIGURACIÓN DE C++ ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- DESCARGAR E INTEGRAR RAYLIB AUTOMÁTICAMENTE ----
include(FetchContent)

# Desactivamos la compilación de ejemplos y juegos de raylib para ahorrar tiempo
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        master 
)

# Hace que raylib esté disponible en el proyecto
FetchContent_MakeAvailable(raylib)

# ---- CONFIGURACIÓN DE NUESTRO EJECUTABLE ----
# 1. Definimos el ejecutable y listamos los archivos de la carpeta src
add_executable(
)

# 2. Le indicamos a CMake dónde buscar NUESTRAS cabeceras (.hpp)
target_include_directories(ml_boundary_visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 3. Enlazamos raylib a nuestro ejecutable (CMake se encarga de sus cabeceras automáticamente)
target_link_libraries(ml_boundary_visualizer PRIVATE raylib)
