#pragma once



#include "../../common/common.h"
#include "../../common/standard_quad.h"



/*

Abstract class for all graphical primitives.

TODO:

Further modifications to the SpriteSheet needed, so we can make ones
with GL_REPEAT for a tex param for some use cases, I guess.

*/

enum class UIPrimitiveType{
	None = 0,
	Pane,
	Label,
	Button,
	Input
};

struct UI_Primitive {

private:

	static void* FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR;

protected:

	//	void* GetStringManager();

public:

	UI_Primitive(
		UIPrimitiveType _type,
		UVRegion _rectangleBorders
	);

	UIPrimitiveType m_Type = UIPrimitiveType::None;
	
	
	glm::vec2 m_PositionRelativeToWindow = glm::vec2(0.f, 0.f);
	float m_Width = 0.f;
	float m_Height = 0.f;



};





struct Pane : UI_Primitive {




	Pane(
		UVRegion _rectangle
	);

};






