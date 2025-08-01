cmake_minimum_required(VERSION 3.10)
project(UniversalLevelEditor)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory to project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Define sources
set(IMGUI_SRC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_glut.cpp
)

set(PROJECT_SRC
    main.cpp
    shaders.cpp
    gui.cpp
    tilemap.cpp
    stamp.cpp
    tileset.cpp
    globals.cpp
    config.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${IMGUI_SRC})

# Include ImGui headers
target_include_directories(${PROJECT_NAME} PRIVATE imgui)

# Platform-specific dependencies
if(WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIR}
    )

    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLEW_LIBRARIES}
        ${GLUT_LIBRARIES}
    )

elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
        GLEW
        GL
        GLU
        glut
    )
endif()
