#include "widget_interface.h"

void WidgetCompositionInterface::RenderWidgetTree(
	Batch* uiBatch,
	glm::vec2 absoluteParentOrigin,
	float baseZLayer,
	float zSubstep
) {
	float currentZLayer = baseZLayer;
	glm::vec2 absoluteCurrentWidgetOrigin = GetAbsolutePosition(absoluteParentOrigin);
	
	SendBackgroundRenderData(uiBatch, absoluteCurrentWidgetOrigin, currentZLayer);
	SendOwnRenderData(uiBatch, absoluteCurrentWidgetOrigin, currentZLayer);

	for (size_t i = 0; i < m_WidgetComposition.size(); i++) {
		m_WidgetComposition[i].get()->RenderWidgetTree(
			uiBatch, 
			absoluteCurrentWidgetOrigin, 
			baseZLayer + zSubstep,
			zSubstep
		);
	}
}

WidgetCompositionInterface* WidgetCompositionInterface::DetectClick(
	glm::vec2 absoluteParentOrigin,
	glm::vec2 mousePosition
) {
	WidgetCompositionInterface* retVal = nullptr;
	glm::vec2 absoluteCurrentWidgetPosition = GetAbsolutePosition(absoluteParentOrigin);

	//	We allow for widgets to exist outside the borders of their parent
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

void WidgetCompositionInterface::SendBackgroundRenderData(
	Batch* uiBatch,
	glm::vec2 absoluteCurrentWidgetOrigin,
	float z
) {
	for (size_t i = 0; i < m_BackgroundGeometry.size(); i++) {
		const FullSprite& sprite = m_BackgroundGeometry[i];
		uiBatch->DrawSprite(
			sprite.instance,
			sprite.position.x + absoluteCurrentWidgetOrigin.x,
			sprite.position.y + absoluteCurrentWidgetOrigin.y,
			0.f,
			z
		);
	}
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
void WidgetCompositionInterface::CalculateBackgroundGeometry() {
	float cornerLength = static_cast<float>(m_BackgroundSkin->cornerLengthPx);
	float verticalSpaceToFill = m_Dimensions.y - 2 * cornerLength;
	float horizontalSpaceToFill = m_Dimensions.x - 2 * cornerLength;

	//DEBUG_ASSERT(verticalSpaceToFill >= 0 && horizontalSpaceToFill >= 0, "Widget\'s background has bad lengths.");

	m_BackgroundGeometry.clear();

	//	Vertical sides
	glm::vec2 leftVerticalSideOrigin = glm::vec2(0, cornerLength);
	SpriteInstance leftVerticalSideInstance = m_BackgroundSkin->instanceArray[4];

	glm::vec2 rightVerticalSideOrigin = glm::vec2(cornerLength + horizontalSpaceToFill, cornerLength);
	SpriteInstance rightVerticalSideInstance = m_BackgroundSkin->instanceArray[5];

	//	Horizontal sides
	glm::vec2 upperHorizontalSideOrigin = glm::vec2(cornerLength, 0);
	SpriteInstance upperHorizontalSideInstance = m_BackgroundSkin->instanceArray[6];

	glm::vec2 lowerHorizontalSideOrigin = glm::vec2(cornerLength, cornerLength + verticalSpaceToFill);
	SpriteInstance lowerHorizontalSideInstance = m_BackgroundSkin->instanceArray[7];

	auto FillSpace = [this](const SpriteInstance& instance, glm::vec2 origin, float totalDistance, bool repeatAboutXaxis) {
		if (totalDistance < 0.f) return;
		
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

		if (std::abs(stepRawValue) < FLOAT_COMPARE_TOLERANCE) {
			return;
		}

		int fullRepeats = static_cast<int>(totalDistance / stepRawValue);
		float lastElement = totalDistance - (fullRepeats * stepRawValue);

		for (int i = 0; i < fullRepeats; i++) {
			glm::vec2 calculatedPosition = origin + step * float(i);
			m_BackgroundGeometry.emplace_back(
				instance,
				calculatedPosition.x,
				calculatedPosition.y
			);
		}

		if (std::abs(lastElement) > FLOAT_COMPARE_TOLERANCE) {
			glm::vec2 calculatedPosition = origin + step * float(fullRepeats);
			if (repeatAboutXaxis) {
				calculatedPosition.x -= stepRawValue - lastElement;
			}
			else {
				calculatedPosition.y -= stepRawValue - lastElement;
			}

			m_BackgroundGeometry.emplace_back(
				instance,
				calculatedPosition.x,
				calculatedPosition.y
			);
		}
	};

	FillSpace(leftVerticalSideInstance, leftVerticalSideOrigin, verticalSpaceToFill, false);
	FillSpace(rightVerticalSideInstance, rightVerticalSideOrigin, verticalSpaceToFill, false);

	FillSpace(upperHorizontalSideInstance, upperHorizontalSideOrigin, horizontalSpaceToFill, true);
	FillSpace(lowerHorizontalSideInstance, lowerHorizontalSideOrigin, horizontalSpaceToFill, true);

	//	Center

	SpriteInstance centerBg = m_BackgroundSkin->instanceArray[8];
	centerBg.dimensions.x = horizontalSpaceToFill;
	centerBg.dimensions.y = verticalSpaceToFill;
	m_BackgroundGeometry.emplace_back(
		centerBg,
		cornerLength,
		cornerLength
	);

	//	Corners

	m_BackgroundGeometry.emplace_back(
		m_BackgroundSkin->instanceArray[0],
		0.f,
		0.f
	);

	m_BackgroundGeometry.emplace_back(
		m_BackgroundSkin->instanceArray[1],
		cornerLength + horizontalSpaceToFill,
		0.f
	);

	m_BackgroundGeometry.emplace_back(
		m_BackgroundSkin->instanceArray[2],
		0.f,
		cornerLength + verticalSpaceToFill
	);

	m_BackgroundGeometry.emplace_back(
		m_BackgroundSkin->instanceArray[3],
		cornerLength + horizontalSpaceToFill,
		cornerLength + verticalSpaceToFill
	);
}

glm::vec2 WidgetCompositionInterface::GetAbsolutePosition(
	glm::vec2 parentOrigin
) const {
	return parentOrigin + m_PositionRelToParent;
}

void WidgetCompositionInterface::SetOnClick(
	std::function<void(EventEmitter* ctx, Window* owningWindow)> fn
) {
	m_OnClickFn = fn;
}

void WidgetCompositionInterface::ApplyChangeToPosition(
	glm::vec2 change
) {
	m_PositionRelToParent += change;
}