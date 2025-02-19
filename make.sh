#!/bin/bash

# Function to build
build() {
    if [ ! -f "CMakeLists.txt" ]; then
        echo "CMakeLists.txt not found"
        exit 1
    fi
    cmake -B build -G Ninja
}

# Function to clean
clean() {
    rm -rf build
}

# Handle arguments
case "$1" in
    "clean")
        clean
        ;;
    "build"|"")
        build
        ;;
    *)
        echo "Usage: $0 [build|clean]"
        exit 1
        ;;
esac
