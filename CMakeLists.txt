cmake_minimum_required(VERSION 3.10)
project(lidar)

set(OpenGL_GL_PREFERENCE GLVND)

add_library(lidar_sdk STATIC IMPORTED)
set_target_properties(lidar_sdk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rplidar_sdk/output/Linux/Release/libsl_lidar_sdk.a)
set_property(TARGET lidar_sdk APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/rplidar_sdk/sdk/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui_lib STATIC ${IMGUI_SOURCES})
target_include_directories(imgui_lib PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

set(SOURCES
    src/main.cpp
    src/lidar_data.cpp
    src/window.cpp
    src/lidar_structs.cpp)

add_executable(lidar_app ${SOURCES})

target_link_libraries(lidar_app PRIVATE 
lidar_sdk
${OPENGL_LIBRARIES}
glfw
${CMAKE_DL_LIBS}
imgui_lib)

target_include_directories(lidar_app PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_SOURCE_DIR}/include
)