#include "TrackballCamera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <p6/p6.h>


TrackballCamera::TrackballCamera(float m_Distance,float m_AngleX,float m_AngleY):m_Distance(5), m_AngleX(0), m_AngleY(0){}

void TrackballCamera::moveFront(float delta){
	m_Distance+=delta;
}
void TrackballCamera::rotateLeft(float degrees){
	m_AngleY+=degrees;
}

void TrackballCamera::rotateUp(float degrees){
	m_AngleX+=degrees;
}

glm::mat4 TrackballCamera::getViewMatrix() const{
	glm::mat4 matrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -m_Distance}); //translate
	// matrix = glm::rotate(matrix, p6::Radians(m_AngleX), {1.f, 0.f, 0.f});
	matrix = glm::rotate(matrix, m_AngleX, {1.f, 0.f, 0.f});
	matrix = glm::rotate(matrix, m_AngleY, {0.f, 1.f, 0.f});
	return matrix;
}
