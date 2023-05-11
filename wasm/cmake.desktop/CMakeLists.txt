cmake_minimum_required(VERSION 3.19)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Find SDL2, SDL2_ttf, and SDL2_image libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# add the emscripten.h /opt/homebrew/Cellar/emscripten/3.1.36/libexec/system/include/emscripten.h
# include_directories(/opt/homebrew/Cellar/emscripten/3.1.36/libexec/system/include/emscripten/)

# Search recursively for source files under src/ folder
file(GLOB_RECURSE SOURCE_FILES "../src/*.cpp")

# Include directories for SDL2, SDL2_ttf, and SDL2_image
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add the executable target
add_executable(app ${SOURCE_FILES})

# Link SDL2, SDL2_ttf, and SDL2_image libraries
target_link_libraries(app ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Add SDL2_image and SDL2_ttf link flags
target_link_libraries(app "-lSDL2_image -lSDL2_ttf")
