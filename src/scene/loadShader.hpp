#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class LoadShader {
private:
    std::filesystem::path        m_vertexShader;
    std::filesystem::path        m_fragmentShader;
    p6::Shader                   m_shader;
    std::map<std::string, GLint> m_uniforms;

public:
    LoadShader();
    LoadShader(std::filesystem::path vertexShader, std::filesystem::path fragmentShader)
        : m_vertexShader(vertexShader), m_fragmentShader(fragmentShader), m_shader(p6::load_shader(vertexShader, fragmentShader)){};

    ~LoadShader() = default;

    void addUniformVariable(const std::string& name)
    {
        GLint uName = glGetUniformLocation(getShaderID(), name.c_str());
        m_uniforms.insert(std::pair<std::string, GLint>(name, uName));
    };

    void use() const
    {
        m_shader.use();
    }

    // getter
    GLuint getShaderID() const
    {
        return m_shader.id();
    }

    // setter

    void setUniform4fv(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(m_uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void setUniform3fv(const std::string& name, const glm::vec3& vector)
    {
        glUniform3fv(m_uniforms[name], 1, glm::value_ptr(vector));
    }

    void setUniform1i(const std::string& name, int value)
    {
        glUniform1i(m_uniforms[name], value);
    }

    void setUniform1f(const std::string& name, float value)
    {
        glUniform1f(m_uniforms[name], value);
    }
};