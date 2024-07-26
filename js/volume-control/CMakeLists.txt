cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(volume-control)

add_definitions(-DNAPI_VERSION=4)

include_directories(${CMAKE_JS_INC})

set(vc_root ../../cpp/install)
include(${vc_root}/VolumeControl.cmake)
set(vc_include ${vc_root}/include)
get_filename_component(vc_dll ${vc_root}/lib/VolumeControl.dll ABSOLUTE)

set(addon_src
    src/main.cpp)

add_library(${PROJECT_NAME} SHARED ${addon_src} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PUBLIC ${vc_include})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} VolumeControl)

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${vc_dll}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
