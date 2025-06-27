#include "ui_primitive.h"


//		--		UI PRIMITIVE		--		//

void* UI_Primitive::FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR = nullptr;


UI_Primitive::UI_Primitive(
	glm::vec2 _dimensions,
	glm::vec2 _relativeToWindow
) :
	m_Dimensions(_dimensions),
	m_PositionRelativeToWindow(_relativeToWindow)
{}






