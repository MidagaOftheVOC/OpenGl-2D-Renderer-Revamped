#include "ui_primitive.h"


//		--		UI PRIMITIVE		--		//

void* UI_Primitive::FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR = nullptr;


UI_Primitive::UI_Primitive(
	UIPrimitiveType _type,
	UVRegion _rectangleBorders
):
	m_Type(_type),
	m_PositionRelativeToWindow(_rectangleBorders.u0, _rectangleBorders.v0),
	m_Width(_rectangleBorders.u1),
	m_Height(_rectangleBorders.v1)
{}






//		--			PANE			--		//


Pane::Pane(
	UVRegion _rectangle
) :
	UI_Primitive(UIPrimitiveType::Pane, _rectangle)
{}





