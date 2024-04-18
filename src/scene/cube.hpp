#pragma once

#include <glm/glm.hpp>
#include "../src/scene/loadShader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Cube {
private:
    std::vector<float>  vertices;
    std::vector<GLuint> indices;
    GLuint              vao, vbo, ibo;
    float               m_size;
    GLuint              texture;

public:
    explicit Cube(float size);

    ~Cube() = default;

    void textures(img::Image& img_load);
    void vectors();
    void vertex();
    void init(img::Image& img_load);

    void draw(glm::vec3 pos, glm::vec3 scale, LoadShader& loadShader, glm::mat4 viewMatrix, glm::mat4 ProjMatrix) const;
};