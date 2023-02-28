#include "FreeflyCamera.hpp"
#include "glm/ext/matrix_transform.hpp"

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = {
        cos(m_Theta) * sin(m_Phi),
        sin(m_Theta),
        cos(m_Theta) * cos(m_Phi),
    };
    m_LeftVector = {
        sin(m_Phi + p6::PI / 2.f),
        0,
        cos(m_Phi + p6::PI / 2.f),
    };
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, glm::vec3{0.f}, m_UpVector);
}