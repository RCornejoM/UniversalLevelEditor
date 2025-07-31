#pragma once

#include <GL/glew.h>

extern const char* VERTEX_SHADER_SRC;
extern const char* FRAGMENT_SHADER_SRC;

GLuint CompileShader(GLenum type, const char* src);
