#pragma once



#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../../batch_types/batch_instance_primitives.h"



/*

Abstract class for all graphical primitives.

*/


struct UI_Primitive {

private:

	static void* FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR;

protected:

	glm::vec2 m_Dimensions;


	//	Drawcalls will be made to the upper left corner of the window object
	//	therefore primitives require offset from that point.
	glm::vec2 m_PositionRelativeToWindow;

public:

	UI_Primitive() {}


	UI_Primitive(
		glm::vec2 _relativeToWindow
	);


	UI_Primitive(
		glm::vec2 _dimensions,
		glm::vec2 _relativeToWindow
	);

private:

	virtual void OnDimensionChange() {}

public:

	void SetDimensions(const glm::vec2& _dimensions);
	void SetPositionRelativeToWindow(const glm::vec2& _posRelativeToWindow);

};










