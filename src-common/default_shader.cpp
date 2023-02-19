#include "default_shader.hpp"
#include "glad/gl.h"

static unsigned int create_default_shader(const char* const vertexShaderSource, const char* const framentShaderSource)
{
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &framentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void bind_default_white_shader()
{
    static unsigned int default_shader = create_default_shader(R"STR(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        
        void main()
        {
           gl_Position = vec4(aPos, 0., 1.);
        }
    )STR",
                                                               R"STR(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.f);
        }
    )STR");
    glUseProgram(default_shader);
}

void bind_default_colored_shader()
{
    static unsigned int default_shader = create_default_shader(R"STR(
        #version 330

        layout(location = 3) in vec2 aVertexPosition;
        layout(location = 8) in vec3 aVertexColor;

        out vec3 vColor;

        void main()
        {
            vColor      = aVertexColor;
            gl_Position = vec4(aVertexPosition, 0, 1);
        }
    )STR",
                                                               R"STR(
        #version 330

        in vec3 vColor;

        out vec3 fFragColor;

        void main()
        {
            fFragColor = vColor;
        }
    )STR");
    glUseProgram(default_shader);
}