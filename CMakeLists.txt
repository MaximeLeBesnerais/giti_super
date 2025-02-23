# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(giti 
    VERSION 3.0.0
    DESCRIPTION "A git utility for better commits"
    LANGUAGES CXX
)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add compile options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find libgit2
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)

# Define the library sources
set(GITI_SOURCES
    src/config/config.cpp
    src/git/types.cpp
    src/git/repository.cpp
    src/cli/parser.cpp
    src/handlers/tool_handler.cpp
)

# Define the library headers
set(GITI_HEADERS
    include/giti/config/config.hpp
    include/giti/git/types.hpp
    include/giti/git/repository.hpp
    include/giti/cli/types.hpp
    include/giti/cli/parser.hpp
    include/giti/handlers/tool_handler.hpp
)

# Add the library target
add_library(giti STATIC ${GITI_SOURCES} ${GITI_HEADERS})

# Set include directories
target_include_directories(giti
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
        ${LIBGIT2_INCLUDE_DIRS}
)

# Link against libgit2
target_link_libraries(giti
    PRIVATE
        ${LIBGIT2_LIBRARIES}
)

# Add the executable
add_executable(giti_exe src/main.cpp)
target_include_directories(giti_exe
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)
target_link_libraries(giti_exe PRIVATE giti)

# Set the output name of the executable
set_target_properties(giti_exe
    PROPERTIES
    OUTPUT_NAME giti
)

# Install rules
include(GNUInstallDirs)

install(TARGETS giti giti_exe
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${GITI_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/giti
)

# Add tests if testing is enabled
option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Generate package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/gitiConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)