#include "widget_interface.h"

void WidgetCompositionInterface::RenderWidgetTree(
	Batch* uiBatch,
	std::vector<TextWithZLayer>& texts,
	glm::vec2 absoluteParentOrigin,
	float baseZLayer,
	float zSubstep,
	float zSubposition
) {
	float currentZLayer = baseZLayer + zSubposition * zSubstep;
	glm::vec2 absoluteCurrentWidgetOrigin = GetAbsolutePosition(absoluteParentOrigin);
	SendOwnRenderData(uiBatch, texts, absoluteCurrentWidgetOrigin, currentZLayer);
	SendStandardisedRenderData(uiBatch);

	for (size_t i = 0; i < m_WidgetComposition.size(); i++) {
		m_WidgetComposition[i].get()->RenderWidgetTree(
			uiBatch, 
			texts, 
			absoluteCurrentWidgetOrigin, 
			baseZLayer, 
			zSubstep, 
			zSubposition + 1
		);
	}
}

WidgetCompositionInterface* WidgetCompositionInterface::DetectClick(
	glm::vec2 absoluteParentOrigin,
	glm::vec2 mousePosition
) {
	WidgetCompositionInterface* retVal = nullptr;
	glm::vec2 absoluteCurrentWidgetPosition = GetAbsolutePosition(absoluteParentOrigin);

	if (PointInRect(mousePosition, absoluteCurrentWidgetPosition, m_Dimensions) && m_IsInteractable) {
		retVal = this;
	}

	WidgetCompositionInterface* nestedWidgetClicked = nullptr;
	for (size_t i = 0; i < m_WidgetComposition.size(); i++) {
		nestedWidgetClicked = m_WidgetComposition[i].get()->DetectClick(absoluteCurrentWidgetPosition, mousePosition);
		if (nestedWidgetClicked) {
			retVal = nestedWidgetClicked;
			break;
		}
	}

	return retVal;
}


/*
+-+----------+-+
|0|    4     |1|
+-+----------+-+
| |          | |
|6|    8     |7|
| |          | |
+-+----------+-+
|2|    5     |3|
+-+----------+-+
*/
void WidgetCompositionInterface::SendStandardisedRenderData(
	Batch* uiBatch
) {
	int totalSpriteCount = 4 + 1;	//	corners are not stretched. Center will always be stretched and only 1 piece
	int totalSubspritesFor45 = 0;
	int totalSubspritesFor67 = 0;

	float cornerLength = m_BackgroundSkin->cornerLengthPx;
	float verticalSpaceToFill = m_Dimensions.y - 2 * cornerLength;
	float horizontalSpaceToFill = m_Dimensions.x - 2 * cornerLength;

	DEBUG_ASSERT(verticalSpaceToFill >= 0 && horizontalSpaceToFill >= 0, "Widget\'s background has bad lengths.");

	//	Vertical sides
	glm::vec2 leftVerticalSideOrigin = glm::vec2(0, cornerLength);
	SpriteInstance leftVerticalSideInstance = m_BackgroundSkin->instanceArray[6];
	
	glm::vec2 rightVerticalSideOrigin = glm::vec2(cornerLength + horizontalSpaceToFill, cornerLength);
	SpriteInstance rightVerticalSideInstance = m_BackgroundSkin->instanceArray[7];

	auto WriteToBatch = [uiBatch](SpriteInstance& instance, glm::vec2 origin) {
		uiBatch->DrawSprite(instance, origin.x, origin.y);
	};

	auto FillSpace = [uiBatch](const SpriteInstance& instance, glm::vec2 origin, float totalDistance, bool repeatAboutXaxis) {
			glm::vec2 step = glm::vec2(0.f, 0.f);
			float stepRawValue = 0;
			if (repeatAboutXaxis) {
				step.x = instance.dimensions.x;
				stepRawValue = step.x;
			}
			else {
				step.y = instance.dimensions.y;
				stepRawValue = step.y;
			}

			int fullRepeats = totalDistance / stepRawValue;
			float lastElement = totalDistance - (fullRepeats * stepRawValue);

			for (int i = 0; i < fullRepeats; i++) {
				glm::vec2 calculatedPosition = origin + step * float(i);
				uiBatch->DrawSprite(
					instance,
					calculatedPosition.x,
					calculatedPosition.y
				);
			}

			if (abs(lastElement) > 0.001) {
				glm::vec2 calculatedPosition = origin + step * float(fullRepeats);
				if (repeatAboutXaxis) {
					calculatedPosition.x -= stepRawValue - lastElement;
				}
				else {
					calculatedPosition.y -= stepRawValue - lastElement;
				}

				uiBatch->DrawSprite(
					instance,
					calculatedPosition.x,
					calculatedPosition.y
				);
			}
		};

	if (verticalSpaceToFill < leftVerticalSideInstance.dimensions.y) {
		leftVerticalSideInstance.dimensions.y = verticalSpaceToFill;
		rightVerticalSideInstance.dimensions.y = verticalSpaceToFill;

		WriteToBatch(leftVerticalSideInstance, leftVerticalSideOrigin);
		WriteToBatch(rightVerticalSideInstance, rightVerticalSideOrigin);
	}
	else {

	}
}

glm::vec2 WidgetCompositionInterface::GetAbsolutePosition(
	glm::vec2 parentOrigin
) const {
	return parentOrigin + m_PositionRelToParent;
}

void WidgetCompositionInterface::SetOnClick(
	std::function<void()> fn
) {
	if (!m_OnClickFn) {
		m_OnClickFn = fn;
	}
}