cmake_minimum_required(VERSION 3.10)
project(UniversalLevelEditor)

# Set CMake policies
cmake_policy(SET CMP0072 NEW)  # Use GLVND if available
set(OpenGL_GL_PREFERENCE "GLVND")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB_RECURSE PROJECT_SRC CONFIGURE_DEPENDS src/*.cpp)

# ImGui sources
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/backends/imgui_impl_glut.cpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${IMGUI_SRC})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/engine
    src/graphics
    src/gui
    external
    external/imgui
    external/imgui/backends
)

# Platform-specific dependencies
find_package(OpenGL REQUIRED)

if(WIN32)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIR}
    )

    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLEW_LIBRARIES}
        ${GLUT_LIBRARIES}
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLEW REQUIRED glew)
    pkg_check_modules(GLUT REQUIRED glut)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
    )

    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_LIBRARY_DIRS}
        ${GLUT_LIBRARY_DIRS}
    )

    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLEW_LIBRARIES}
        ${GLUT_LIBRARIES}
    )
endif()
