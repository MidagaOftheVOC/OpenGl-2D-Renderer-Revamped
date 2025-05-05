#pragma once


#include "../common/common.h"


class Camera {

	glm::vec2 m_Position;
	glm::vec2 m_ViewPortSize;
	float m_Zoom = 1.f;

	glm::mat4 m_ModelMatrix;		//	model -> world space
	glm::mat4 m_ViewMatrix;			//	world space -> view space (POV of camera)
	glm::mat4 m_ProjectionMatrix;	//	view space -> 2d screen coordinates


public:

	Camera() {}


	
	glm::vec2 GetPosition() { return m_Position; }
};

