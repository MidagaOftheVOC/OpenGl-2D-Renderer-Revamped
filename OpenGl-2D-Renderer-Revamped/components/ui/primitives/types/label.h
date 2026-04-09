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

public:

	const std::u32string& GetString() const { return m_TextObject.GetTextString(); }
	const Text* GetText() const { return &m_TextObject; };

	virtual void AppendWidgetRenderDataToArrays(
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		std::vector<SpriteInformation>& OUT_batchSpriteInformation,
		float _xOffset,
		float _yOffset
	) {}

	virtual void AppendWidgetTextDataToArray(
		std::vector<Text>& OUT_textArraysToRender
	);

	virtual ~Label() {}
};

