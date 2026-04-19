#include "label.h"

void Label::SendOwnRenderData(
	Batch* uiBatch,
	glm::vec2 absoluteCurrentWidgetOrigin,
	float z
) const {
	uiBatch->DrawText(&m_StoredText, absoluteCurrentWidgetOrigin.x, absoluteCurrentWidgetOrigin.y, z);
}