cmake_minimum_required(VERSION 3.1)

project(pokezoo)

# add a property to edit wasm through ccmake
set(WASM OFF CACHE BOOL "Build for WebAssembly")

if (WASM)
  # WASM build configuration
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmake.wasm)
else()
    # run a CMakeLists.txt file in the subdirectory
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmake.desktop)
endif()
