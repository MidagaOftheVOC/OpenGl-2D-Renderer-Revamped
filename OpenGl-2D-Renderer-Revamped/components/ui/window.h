#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include <vector>
#include <memory>
#include <unordered_map>


#include "ui_primitive.h"




class Window {

	ID m_WindowsID;


	const ID m_Paren = 0;


	//	Relative to top left corner of screen
	glm::vec2 m_WindowPosition = glm::vec2(0.f, 0.f);


	glm::vec2 m_WindowDimensions = glm::vec2(200.f, 80.f);

private:	// UI PRIMITIVE DATA

	bool m_HasPane = true;
	Pane m_WindowPane;



public:

	Window(
		const ID _id
	);


	
public:



	const Pane& GetPane() const { return m_WindowPane; }

};




