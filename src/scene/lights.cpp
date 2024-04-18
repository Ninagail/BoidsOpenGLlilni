#include "../src/scene/lights.hpp"
#include "glm/fwd.hpp"

void Light::drawLightScene(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& shader)
{
    // glm::mat4 ViewMatrixLight = viewMatrix;
    shader.use();
    shader.setUniform3fv("uKd", glm::vec3(1.0f, 0.95f, 0.8f));
    shader.setUniform3fv("uKs", glm::vec3(1.0f, 0.95f, 0.8f));
    shader.setUniform1f("uShininess", 0.6);

    glm::vec4 LightPos = viewMatrix * glm::vec4(pos, 1.0);
    shader.setUniform3fv("uLightPos_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    shader.setUniform3fv("uLightIntensity", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    shader.setUniform4fv("uMVPMatrix", projMatrix * viewMatrix);
    shader.setUniform4fv("uMVMatrix", viewMatrix);
    shader.setUniform4fv("uNormalMatrix", NormalMatrix);
}

void Light::drawLightPlayer(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& shader)
{
    glm::vec4 LightPos = viewMatrix * glm::vec4(pos, 1.0);
    shader.setUniform3fv("uLightPos2_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    shader.setUniform3fv("uLightIntensity2", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    shader.setUniform4fv("uMVPMatrix", projMatrix * viewMatrix);
    shader.setUniform4fv("uMVMatrix", viewMatrix);
    shader.setUniform4fv("uNormalMatrix", NormalMatrix);
}