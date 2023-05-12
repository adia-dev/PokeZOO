#!/bin/bash

# Set default values
CMAKE_DIR=""
BUILD_DIR=""
RUN_PROJECT=false
CLEAN_BUILD=false

# Handle arguments
while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
    --sdl)
        CMAKE_DIR="cmake.desktop"
        BUILD_DIR="build.desktop"
        ;;
    --wasm)
        CMAKE_DIR="cmake.wasm"
        BUILD_DIR="build.wasm"
        ;;
    --run)
        if [[ "$CMAKE_DIR" == "cmake.desktop" ]]; then
            RUN_PROJECT=true
        else
            echo "Cannot run project with --wasm argument."
            exit 1
        fi
        ;;
    --rm)
        CLEAN_BUILD=true
        ;;

    --deploy)
        if [[ "$CMAKE_DIR" == "cmake.desktop" ]]; then
          echo "Cannot deploy an SDL executable project, run with --wasm argument."
          exit 1
        else
          SHOULD_DEPLOY=true
        fi
        ;;
    *)
        echo "Invalid argument: $1"
        exit 1
        ;;
    esac

    shift
done

# Check for required arguments
if [ -z "$CMAKE_DIR" ] || [ -z "$BUILD_DIR" ]; then
    echo "Please provide an argument of either --sdl or --wasm"
    exit 1
fi

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    rm -rf "$BUILD_DIR"/*
    echo "Build directory cleaned."
    exit 0
fi

# Create build directory if it does not exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

# Build the project
if [[ "$CMAKE_DIR" == "cmake.desktop" ]]; then
    cmake -S "$CMAKE_DIR" -B "$BUILD_DIR" && make -C "$BUILD_DIR"
elif [[ "$CMAKE_DIR" == "cmake.wasm" ]]; then
    emcmake cmake -S "$CMAKE_DIR" -B "$BUILD_DIR" && make -C "$BUILD_DIR"
fi

# Check for errors
if [ $? -eq 0 ]; then
    echo "\033[0;32mBuild succeeded!\033[0m"
else
    echo "\033[0;31mBuild failed.\033[0m"
    exit 1
fi

# Run the project if requested
if [ "$RUN_PROJECT" = true ]; then
    cd "$BUILD_DIR"
    ./app
fi

if [ "$SHOULD_DEPLOY" = true ]; then
    cd ../frontend
    fly deploy
fi
