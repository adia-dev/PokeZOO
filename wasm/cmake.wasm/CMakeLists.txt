cmake_minimum_required(VERSION 3.1)

# Define customizable variables
set(MY_OPTIMIZATION_LEVEL "O1" CACHE STRING "The optimization level")
set(MY_EXPORTED_FUNCTIONS "_mainf" CACHE STRING "The exported functions")
set(MY_TOTAL_MEMORY "67108864" CACHE STRING "The total memory")

# Set the default output directory for the built files
set(DEFAULT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../dist")

# Allow the user to override the output directory using a CMake variable or an environment variable
set(OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}" CACHE PATH "The output directory")
if(NOT OUTPUT_DIRECTORY)
    message(STATUS "OUTPUT_DIRECTORY not provided. Set it using the OUTPUT_DIRECTORY CMake variable or the environment variable.")
    message(STATUS "To set the output directory, run the following command: cmake -DOUTPUT_DIRECTORY=<path> .")
    message(STATUS "Using the default output directory: ${DEFAULT_OUTPUT_DIRECTORY}")
    set(OUTPUT_DIRECTORY "${DEFAULT_OUTPUT_DIRECTORY}")
endif()

# Create the output directory if it does not exist
if(NOT EXISTS "${OUTPUT_DIRECTORY}")
    message(STATUS "Creating the output directory: ${OUTPUT_DIRECTORY}")
    file(MAKE_DIRECTORY "${OUTPUT_DIRECTORY}")
endif()

# Set a property to specify the name of the output file
set(OUTPUT_NAME "pokezoo" CACHE STRING "The name of the output file")


# Define source files
file(GLOB_RECURSE SOURCES 
    ../src/*.cpp
    ../src/**/*.cpp
)

# Create the executable
add_executable(${OUTPUT_NAME} ${SOURCES})

# Define CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -${MY_OPTIMIZATION_LEVEL} \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS='[\"png\", \"jpg\"]' \
    -s USE_SDL_TTF=2 \
    -s EXPORTED_FUNCTIONS='[\"${MY_EXPORTED_FUNCTIONS}\"]' \
    -s NO_EXIT_RUNTIME=1 \
    -s TOTAL_MEMORY=${MY_TOTAL_MEMORY} \
    -s --preload-file ../src/assets \
    -s \"EXPORTED_RUNTIME_METHODS=['ccall']\""
)

# Add compile definition
target_compile_definitions(${OUTPUT_NAME} PUBLIC __EMSCRIPTEN__)


# Copy the built files to the specified output directory
add_custom_command(TARGET ${OUTPUT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}.js "${OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}.wasm "${OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}.data "${OUTPUT_DIRECTORY}"
)

project(pokezoo)
