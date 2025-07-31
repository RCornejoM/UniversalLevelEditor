# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iimgui

# Linker flags
LDFLAGS = -lGLEW -lGL -lGLU -lglut

# ImGui source files
IMGUI_SRC = imgui/imgui.cpp \
            imgui/imgui_draw.cpp \
            imgui/imgui_tables.cpp \
            imgui/imgui_widgets.cpp \
            imgui/imgui_impl_opengl3.cpp \
            imgui/imgui_impl_glut.cpp

# Project source files
SRC = main.cpp shaders.cpp gui.cpp tilemap.cpp stamp.cpp tileset.cpp globals.cpp config.cpp $(IMGUI_SRC)

# Target binary
TARGET = Universal-Level-Editor

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
