#include "shaders.h"
#include <GL/glew.h>
#include <iostream>

const char* VERTEX_SHADER_SRC = R"GLSL(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
out vec2 TexCoord;
void main() {
    TexCoord = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)GLSL";

const char* FRAGMENT_SHADER_SRC = R"GLSL(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D uTex;
void main() {
    FragColor = texture(uTex, TexCoord);
}
)GLSL";

GLuint CompileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error: " << log << "\n";
    }
    return shader;
}
