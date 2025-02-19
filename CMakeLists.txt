cmake_minimum_required(VERSION 3.14)
project(giti VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Enable strict warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Define directories
set(GITI_INCLUDE_DIR "include")
set(THIRD_PARTY_DIR "third_party")
set(SRC_DIR "src")

# Find libgit2
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)

# Add Executable (Named "giti")
file(GLOB_RECURSE GITI_SOURCES "${SRC_DIR}/*.cpp")
add_executable(giti ${GITI_SOURCES})

# Include Directories (Scoped to Target)
target_include_directories(giti PRIVATE ${GITI_INCLUDE_DIR} ${THIRD_PARTY_DIR} ${SRC_DIR} ${LIBGIT2_INCLUDE_DIRS})

# Link libgit2
target_link_libraries(giti PRIVATE ${LIBGIT2_LIBRARIES})
target_compile_options(giti PRIVATE ${LIBGIT2_CFLAGS_OTHER})

# Install Instructions
install(TARGETS giti DESTINATION bin)