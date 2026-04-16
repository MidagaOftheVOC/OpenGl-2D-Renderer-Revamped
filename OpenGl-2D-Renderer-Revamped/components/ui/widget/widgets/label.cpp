#include "label.h"

void Label::SendOwnRenderData(
	Batch* uiBatch,
	std::vector<TextWithZLayer>& texts,
	glm::vec2 absoluteCurrentWidgetOrigin,
	float z
) const {
	texts.emplace_back(&m_StoredText, absoluteCurrentWidgetOrigin.x, absoluteCurrentWidgetOrigin.y, z);
}