#pragma once


#include "../ui_primitive.h"
#include "../../text.h"


struct Label : public UI_Primitive {

	Text m_TextObject;

public:

	Label() : UI_Primitive() {}

	
	Label(
		const char32_t _string
	);


	Label(
		const Text& _textObject
	);
	


	void UpdateBuffers();

public:

	const std::u32string& GetString() const;
	Text& GetText();

};

