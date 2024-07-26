cmake_minimum_required(VERSION 3.5.0)

# project settings
project(VolumeControl CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# lib sources
set(lib_dir lib/win)
set(lib_source_dir ${lib_dir}/src)
set(lib_header_dir ${lib_dir}/include)
set(lib_public_dir lib/interface)
set(lib_generated_header_dir ${CMAKE_BINARY_DIR}/generated)

set(lib_export_header
    ${lib_generated_header_dir}/vc_export.hpp)

set(lib_public_headers
    ${lib_public_dir}/ivolume.hpp
    ${lib_public_dir}/idevice.hpp
    ${lib_public_dir}/isession.hpp
    ${lib_export_header})

set(lib_sources
    ${lib_source_dir}/volume.cpp
    ${lib_source_dir}/device.cpp
    ${lib_source_dir}/session.cpp
    ${lib_source_dir}/helper.cpp)

# lib target
include_directories(${lib_generated_header_dir})
add_library(VolumeControl SHARED ${lib_public_headers} ${lib_sources})
target_include_directories(VolumeControl PRIVATE ${lib_public_dir} ${lib_header_dir})

include(GenerateExportHeader)
generate_export_header(VolumeControl
    BASE_NAME VolumeControl
    EXPORT_MACRO_NAME VC_EXPORT
    EXPORT_FILE_NAME ${lib_export_header}
    STATIC_DEFINE VC_BUILT_AS_STATIC)

# install settings
set_target_properties(VolumeControl PROPERTIES PUBLIC_HEADER "${lib_public_headers}")

include(GNUInstallDirs)
install(TARGETS VolumeControl
    EXPORT VolumeControl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT VolumeControl DESTINATION ${CMAKE_INSTALL_PREFIX})

# executable sources
set(cli_dir cli)
set(cli_source_dir ${cli_dir}/src)

set(cli_sources
    ${cli_source_dir}/main.cpp)

# executable target
add_executable(VolumeCLI ${cli_sources})
target_include_directories(VolumeCLI PUBLIC ${lib_public_dir})
target_link_libraries(VolumeCLI VolumeControl)
set(VS_STARTUP_PROJECT VolumeCLI)
