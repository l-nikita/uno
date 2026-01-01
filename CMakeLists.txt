cmake_minimum_required(VERSION 3.5...4.0)

set(CMAKE_TOOLCHAIN_FILE "C:/FILES/Programs/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(Uno VERSION 1.0)

# Common
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/bin/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/lib/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/lib/Debug)

# Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/bin/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/lib/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/lib/Release)

# ------------------ cpp ------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ------------------ packages ------------------
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# ----------------------------------------------
add_executable(uno)

target_sources(uno PRIVATE
    main.cpp
    game.cpp
    game.hpp
)

target_link_libraries(uno PRIVATE 	
	SDL3::SDL3
	SDL3_image::SDL3_image-shared
)

# ----------------------------------------------
set_target_properties(uno PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "uno")