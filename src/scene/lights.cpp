#include "../src/scene/lights.hpp"
#include "glm/fwd.hpp"

void Light::drawLightScene(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& shader)
{
    // glm::mat4 ViewMatrixLight = viewMatrix;
    shader.use();
    shader.setUniform3fv("uKd", glm::vec3(1.0f, 0.8f, 0.4f));
    shader.setUniform3fv("uKs", glm::vec3(1.0f, 0.8f, 0.4f));
    shader.setUniform1f("uShininess", 0.4);

    glm::vec4 LightPos = viewMatrix * glm::vec4(pos, 1.0);
    shader.setUniform3fv("uLightPos_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    shader.setUniform3fv("uLightIntensity", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    shader.setUniform4fv("uMVPMatrix", projMatrix * viewMatrix);
    shader.setUniform4fv("uMVMatrix", viewMatrix);
    shader.setUniform4fv("uNormalMatrix", NormalMatrix);
}

void Light::drawLightPerson(glm::vec3 cameraPosition, glm::mat4 projMatrix, glm::mat4 viewMatrix, LoadShader& shader)
{
    // Transformer la position de la lumière dans l'espace de vue de la caméra
    glm::vec4 LightPos = viewMatrix * glm::vec4(cameraPosition, 1.0);
    shader.setUniform3fv("uLightPos2_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    // Passer l'intensité de la lumière
    shader.setUniform3fv("uLightIntensity2", this->m_intensity);

    // Calculer la matrice normale
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    // Passer les matrices au shader
    shader.setUniform4fv("uMVPMatrix", projMatrix * viewMatrix);
    shader.setUniform4fv("uMVMatrix", viewMatrix);
    shader.setUniform4fv("uNormalMatrix", NormalMatrix);
}
