#pragma once

#include <glm/glm.hpp>
#include "../src/model/model.hpp"
#include "../src/scene/loadShader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Person {
private:
    glm::vec3 m_Position; // camera position
    float     m_Phi;      // coord spherique de F
    float     m_Theta;    // coord spherique de F
    float     m_RotationAngle;

    glm::vec3 m_FrontVector; // vecteur F
    glm::vec3 m_LeftVector;  // vecteur L
    glm::vec3 m_UpVector;    // vecteur U

    void computeDirectionVectors();

public:
    Person();

    Person(glm::vec3 position, float phi, float theta);

    ~Person() = default;

    void setPosition(glm::vec3 posPlayer);

    glm::vec3 getPosition();

    void setRotationAngle(float rotationAngle);

    float getRotationAngle() const;

    void moveLeft(float t);

    void moveFront(float t);

    void rotateLeft(float degrees);

    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;

    void drawPerson(Model& model, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, LoadShader& loadShader, GLuint textName);
};

void cameraOption(Person& camera, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx);