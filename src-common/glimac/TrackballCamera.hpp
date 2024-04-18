#pragma once

#include <glm/glm.hpp>
#include "glm/fwd.hpp"

class TrackballCamera {
	private:
	float m_Distance;
	float m_AngleX;
	float m_AngleY;

	public:
	TrackballCamera(float m_Distance,float m_AngleX,float m_AngleY);

	void moveFront(float delta);
	void rotateLeft(float degrees);
	void rotateUp(float degrees);

	glm::mat4 getViewMatrix() const;

};