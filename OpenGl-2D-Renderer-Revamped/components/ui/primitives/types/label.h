#pragma once


#include "../ui_primitive.h"
#include "../../text.h"


struct Label : public UI_Primitive {

	Text m_TextObject;

public:

	Label() : UI_Primitive() {}

	Label(
		const Text& _textObject,
		glm::vec2 _offsetFromWindowCorner
	);

	virtual void PostAttachment(WidgetWindowData _data);

	void UpdateBuffers();

public:

	const std::u32string& GetString() const;
	const Text* GetText() const { return &m_TextObject; };

	virtual void DoAction() {}

};

