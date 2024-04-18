#include "FreeflyCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <p6/p6.h>


Freefly::Freefly():m_Position(glm::vec3(0., 0., 0.)), m_Phi(p6::PI), m_Theta(0.){
	this->computeDirectionVectors();
};

Freefly::Freefly(glm::vec3 position, float phi, float theta):m_Position(position), m_Phi(phi), m_Theta(theta){
	this->computeDirectionVectors();
};

void Freefly::computeDirectionVectors(){
	m_FrontVector = glm::vec3(glm::cos(m_Theta)*glm::sin(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta)*glm::cos(m_Phi));
	m_LeftVector = glm::vec3(glm::sin(m_Phi+(p6::PI/2)), 0, glm::cos(m_Phi+(p6::PI/2)));
	m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void Freefly::moveLeft(float t){
	m_Position += t*m_LeftVector;
	computeDirectionVectors();
}
void Freefly::moveFront(float t){
	m_Position += t*m_FrontVector;
	computeDirectionVectors();
}

void Freefly::rotateLeft(float degrees){
	m_Theta += glm::radians(degrees);
	computeDirectionVectors();
}
void Freefly::rotateUp(float degrees){
	m_Phi += glm::radians(degrees);
	computeDirectionVectors();
}

glm::mat4 Freefly::getViewMatrix() const{
	return glm::lookAt(m_Position, m_FrontVector, m_UpVector);
}
