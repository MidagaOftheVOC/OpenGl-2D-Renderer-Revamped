#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include <vector>
#include <memory>
#include <unordered_map>


#include "primitives/types/pane.h"


class Window {

	ID m_ID = 0;


	ID m_Parent = 0;

		
	//	Relative to top left corner of screen
	glm::vec2 m_WindowPosition = glm::vec2(0.f, 0.f);


	glm::vec2 m_WindowDimensions = glm::vec2(200.f, 80.f);

private:	// UI PRIMITIVE DATA

	bool m_HasPane = false;
	Pane m_WindowPane;

public:

	Window() {}


	Window(
		ID _windowId
	);


	//	If set to nullptr, pane will be removed
	void SetPane(
		Pane&& _pane
	);
	
public:

	//	Also changes the pane's dimensions, if there is one
	void SetDimensions(
		glm::vec2 _dimensions
	);


	void SetPosition(
		glm::vec2 _position
	);

	ID GetID() const { return m_ID; }
	const Pane& GetPane() const { return m_WindowPane; }
	const bool HasPane() const { return m_HasPane; }
	const glm::vec2& GetWinPosition() const { return m_WindowPosition; }

};




