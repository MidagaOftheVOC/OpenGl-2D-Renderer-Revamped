#pragma once
#include "../ui_primitive.h"
#include <functional>


class Button : public UI_Primitive {

	std::function<void()> m_StoredActionLambda;

public:

	Button(
		glm::vec2 dimensions,
		glm::vec2 relativeToWindow,
		std::function<void()> actionLambda
	);

public:

	virtual void DoAction();
	virtual void AppendWidgetRenderDataToArray(std::vector<float>& OUT_rects, std::vector<TextWithZLayer>& OUT_texts, float zLayer);
	virtual void PostAttachment(WidgetWindowData _data);

	virtual ~Button() {}
};

