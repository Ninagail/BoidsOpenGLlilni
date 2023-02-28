#pragma once

#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"
class TrackballCamera {
public:
    void moveFront(float delta)
    {
        m_Distance -= delta;
    }
    void rotateLeft(float degrees)
    {
        m_AngleX += 10 * degrees;
    }
    void rotateUp(float degrees)
    {
        m_AngleY += 10 * degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        auto res = glm::mat4{1.f};
        res      = glm::rotate(res, glm::radians(m_AngleX), {1.f, 0.f, 0.f});
        res      = glm::rotate(res, glm::radians(m_AngleY), {0.f, 1.f, 0.f});
        res      = glm::translate(res, {0.f, 0.f, -m_Distance});

        return res;
    }

private:
    float m_Distance{};
    float m_AngleX{};
    float m_AngleY{};
};