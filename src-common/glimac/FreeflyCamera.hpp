#pragma once
#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"

class FreeflyCamera {
public:
    FreeflyCamera()
    {
        computeDirectionVectors();
    }

    void computeDirectionVectors();

    void moveLeft(float t)
    {
        m_Position += m_LeftVector * t;
    }
    void moveFront(float t)
    {
        m_Position += m_FrontVector * t;
    }
    void rotateLeft(float degrees)
    {
        m_Phi += 10 * glm::radians(degrees);
        computeDirectionVectors();
    }
    void rotateUp(float degrees)
    {
        m_Theta += 10 * glm::radians(degrees);
        computeDirectionVectors();
    }
    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 m_Position{0.f};
    float     m_Phi{p6::PI};
    float     m_Theta{0.f};

    glm::vec3 m_FrontVector{};
    glm::vec3 m_LeftVector{};
    glm::vec3 m_UpVector{};
};