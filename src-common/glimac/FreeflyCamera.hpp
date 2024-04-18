#pragma once

#include <glm/glm.hpp>
#include "glm/fwd.hpp"


class Freefly {
	private:
	glm::vec3 m_Position; //camera position
	float m_Phi; //coord spherique de F
	float m_Theta; //coord spherique de F

	glm::vec3 m_FrontVector; //vecteur F
	glm::vec3 m_LeftVector; //vecteur L
	glm::vec3 m_UpVector; //vecteur U

	void computeDirectionVectors();

	public:
	Freefly();
	Freefly(glm::vec3 position, float phi, float theta);

	void moveLeft(float t); //deplacer la cam le long du vecteur L
	void moveFront(float t); //deplacer la cam le long di vecteur F

	void rotateLeft(float degrees); //modifie la rotation de la cam axe horizontal
	void rotateUp(float degrees); //modifie la rotation de la cam axe vertical

	glm::mat4 getViewMatrix() const;



};