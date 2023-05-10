cmake_minimum_required(VERSION 3.1)

project(pokezoo)

file(GLOB_RECURSE SOURCES 
    ../src/*.cpp
    ../src/**/*.cpp
)

add_executable(out ${SOURCES})

# Define customizable variables
set(MY_OPTIMIZATION_LEVEL "O1" CACHE STRING "The optimization level")
set(MY_EXPORTED_FUNCTIONS "_mainf" CACHE STRING "The exported functions")
set(MY_TOTAL_MEMORY "67108864" CACHE STRING "The total memory")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# Define CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  \
    -${MY_OPTIMIZATION_LEVEL}  \
    -s USE_SDL=2  \
    -s USE_SDL_IMAGE=2  \
    -s SDL2_IMAGE_FORMATS='[\"png\", \"jpg\"]'  \
    -s USE_SDL_TTF=2  \
    -s EXPORTED_FUNCTIONS='[\"${MY_EXPORTED_FUNCTIONS}\"]'  \
    -s NO_EXIT_RUNTIME=1  \
    -s TOTAL_MEMORY=${MY_TOTAL_MEMORY}  \
    -s --preload-file ../src/assets  \
    -s \"EXPORTED_RUNTIME_METHODS=['ccall']\""
)

add_custom_command(TARGET out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:out> /Users/abdoulayedia/Projects/School/AL_2/NodeJS/PokeZOO/frontend/public/wasm
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/out.wasm /Users/abdoulayedia/Projects/School/AL_2/NodeJS/PokeZOO/frontend/public/wasm
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/out.data /Users/abdoulayedia/Projects/School/AL_2/NodeJS/PokeZOO/frontend/public/wasm
)