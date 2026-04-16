#pragma once
#include "../ui_primitive.h"
#include "pane.h"

#include <functional>


class Button : public UI_Primitive {

	std::function<void()> m_StoredActionLambda;

public:

	Button(
		glm::vec2 dimensions,
		glm::vec2 relativeToWindow,
		Text label,
		std::function<void()> actionLambda
	);

public:

	virtual void AppendWidgetRenderDataToArrays(
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		std::vector<SpriteInformation>& OUT_batchSpriteInformation,
		float _xOffset,
		float _yOffset
	) {
	}

	virtual void AppendWidgetTextDataToArray(
		std::vector<Text>& OUT_textArraysToRender
	) {
	}


	virtual void OnClick();

	virtual ~Button() {}
};

