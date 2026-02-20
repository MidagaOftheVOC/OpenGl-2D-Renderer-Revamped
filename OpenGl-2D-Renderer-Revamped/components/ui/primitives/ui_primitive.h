#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include "../../batch_types/batch_instance_primitives.h"

/*

Abstract class for all graphical primitives.

*/

struct WidgetWindowData {
	glm::vec2 WindowDimensions;
};

struct UI_Primitive {

private:

	static void* FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR;

protected:

	glm::vec2 m_Dimensions = { 0.f, 0.f };


	//	Drawcalls will be made to the upper left corner of the window object
	//	therefore primitives require offset from that point.
	glm::vec2 m_PositionRelativeToWindow = { 0.f, 0.f };

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

	virtual void OnDimensionChange() {}	//<<< why is this here??

public:

	float GetXOffset() const { return m_PositionRelativeToWindow.x; }
	float GetYOffset() const { return m_PositionRelativeToWindow.y; }

	void SetDimensions(const glm::vec2& _dimensions);
	void SetPositionRelativeToWindow(const glm::vec2& _posRelativeToWindow);
	virtual void PostAttachment(WidgetWindowData _data) = 0;

};










