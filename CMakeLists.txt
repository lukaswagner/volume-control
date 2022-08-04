cmake_minimum_required(VERSION 3.0.0)

# project settings
project(VolumeControl CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# sources
set(source_dir src)
set(header_dir include)

set(sources
    ${source_dir}/main.cpp
    ${source_dir}/volume.cpp
    ${source_dir}/device.cpp
    ${source_dir}/session.cpp
    ${source_dir}/helper.cpp)

# executables
add_executable(VolumeControl ${sources})
target_include_directories(VolumeControl PRIVATE ${header_dir})
