#include "../src/person/person.hpp"
#include <p6/p6.h>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

void Person::setPosition(glm::vec3 posPerson)
{
    m_Position = posPerson;
}

glm::vec3 Person::getPosition()
{
    return m_Position;
}

float Person::getRotationAngle() const
{
    return m_RotationAngle;
}

void Person::setRotationAngle(float rotationAngle)
{
    m_RotationAngle = rotationAngle;
}

Person::Person()
    : m_Position(glm::vec3(0., 0.0, -5.)), m_Phi(p6::PI), m_Theta(0.), m_RotationAngle(0.)
{
    this->computeDirectionVectors();
};

Person::Person(glm::vec3 position, float phi, float theta)
    : m_Position(position), m_Phi(phi), m_Theta(theta)
{
    this->computeDirectionVectors();
};

void Person::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos(m_Theta) * glm::sin(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta) * glm::cos(m_Phi));
    m_LeftVector  = glm::vec3(glm::sin(m_Phi + (p6::PI / 2)), 0, glm::cos(m_Phi + (p6::PI / 2)));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
}

void Person::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
    computeDirectionVectors();
}
void Person::moveFront(float t)
{
    m_Position += t * m_FrontVector;
    computeDirectionVectors();
}

void Person::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}
void Person::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 Person::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void cameraOption(Person& person, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx)
{
    if (right)
    {
        person.rotateLeft(-0.5f);
    }
    if (left)
    {
        person.rotateLeft(+0.5f);
    }
    if (up)
    {
        person.moveFront(0.05f);
    }
    if (down)
    {
        person.moveFront(-0.05f);
    }

    ctx.key_pressed = [&right, &up, &left, &down](p6::Key key) {
        if (key.logical == "k")
        {
            right = true;
        }
        if (key.logical == "h")
        {
            left = true;
        }
        if (key.logical == "u")
        {
            up = true;
        }
        if (key.logical == "n")
        {
            down = true;
        }
    };

    ctx.key_released = [&right, &up, &left, &down](p6::Key key) {
        if (key.logical == "k")
        {
            right = false;
        }
        if (key.logical == "h")
        {
            left = false;
        }
        if (key.logical == "u")
        {
            up = false;
        }
        if (key.logical == "n")
        {
            down = false;
        }
    };

    ctx.mouse_dragged = [&person](const p6::MouseDrag& button) {
        person.rotateLeft(button.delta.x * 5);
        person.rotateUp(-button.delta.y * 5);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        person.moveFront(-scroll.dy);
    };
}

void Person::drawPerson(Model& model, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, LoadShader& loadShader, GLuint textName)
{
    // glm::mat4 ViewMatrix = camera->getViewMatrix();
    ViewMatrix = glm::translate(ViewMatrix, m_Position);
    // ViewMatrix = glm::rotate(ViewMatrix, 45.f, glm::vec3(1.,0.,0.));
    ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0., -4.8, 2.0));

    ViewMatrix             = glm::scale(ViewMatrix, glm::vec3(0.12, 0.12, -0.12));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix));

    loadShader.setUniform1i("uTextLadybug", 0);
    loadShader.setUniform4fv("uMVPMatrix", ProjMatrix * ViewMatrix);
    loadShader.setUniform4fv("uMVMatrix", ViewMatrix);
    loadShader.setUniform4fv("uNormalMatrix", NormalMatrix);

    model.drawArray(textName);
}
