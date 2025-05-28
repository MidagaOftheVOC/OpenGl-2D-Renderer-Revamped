#pragma once



#include "../../common/common.h"
#include "../../common/standard_quad.h"



/*

Abstract class for all graphical primitives.

*/

struct UI_Primitive {

	float m_LeftBound	= -1.f;
	float m_RightBound	= -1.f;
	float m_UpperBound	= -1.f;
	float m_LowerBound	= -1.f;

	
	glm::vec2 m_PositionRelativeToLogicalAbstractUnit = glm::vec2(0.f, 0.f);



};
