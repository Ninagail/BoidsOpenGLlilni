#pragma once

#include <glm/glm.hpp>
#include "../src/scene/loadShader.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"


class Light {
private:
    glm::vec3 m_intensity;

public:
    Light(glm::vec3 intensity)
        : m_intensity(intensity){};

    ~Light() = default;

    // méthodes
    void drawLightScene(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& loadshader);
    void drawLightPlayer(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& loadshader);

    // Getters
    glm::vec3 get_intensity() const { return m_intensity; }

    // Setters
    void set_intensity(glm::vec3 intensity) { m_intensity = intensity; }
};
