# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.24)

project(MksvEngine
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable RTTI
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

# Add Microsoft static analyzer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /analyze /analyze:external- /analyze:projectdirectory ${CMAKE_CURRENT_SOURCE_DIR} /analyze:ruleset analyzer.ruleset")

# Multithreaded compilation
add_compile_options(/MP)

# Warning level + warning as errors
add_compile_options(/W4 /WX)

include(Print.cmake)
include(ClangFormat.cmake)

# Include sub-projects.
add_subdirectory("mksv_engine")
add_subdirectory("sandbox")

set_directory_properties(PROPERTIES
    VS_STARTUP_PROJECT "sandbox"
)
