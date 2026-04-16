#include "ui_manager.h"

void UIManager::SetZSpaceDistribution(
	float zClose,
	float zFurther
) {
	m_CloserBoundOfZLayerDistribution = zClose;
	m_FurtherBoundOfZLayerDistribution = zFurther;
}
