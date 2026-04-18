#include "label.h"

void Label::SendOwnRenderData(
	Batch* uiBatch,
	Batch* texts,
	glm::vec2 absoluteCurrentWidgetOrigin,
	float z
) const {
	texts->DrawText(&m_StoredText, absoluteCurrentWidgetOrigin.x, absoluteCurrentWidgetOrigin.y, z);
}