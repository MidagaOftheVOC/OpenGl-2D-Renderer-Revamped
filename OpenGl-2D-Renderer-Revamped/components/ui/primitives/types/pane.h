#pragma once


#include "../ui_primitive.h"






struct Pane : public UI_Primitive {
private:

	float m_PositionPairArray[18];
	float m_DimensionPairArray[18];

	const SpriteInformation* m_Skin = nullptr;

	float m_CornerSidePx = 20.f;

public:

	Pane() : UI_Primitive() {}

	Pane(
		glm::vec2 _dimensions,
		float _cornerLengthPx,
		bool _clickable = false
	);

	//	This function outputs vertex coord data for the FreeBatch object.
	//	SpriteInformation data will be added by the UI manager immediately after.
	//	This is made to avoid unnecessary complication while allowing the UI managre to set
	//	custom indices for the corners, sides, and so on.

private:

	void AppendPaneBatchingData(
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		std::vector<SpriteInformation>& OUT_batchSpriteInformation,
		float _xOffset,
		float _yOffset
	) const;

	void UpdateArrays();

	virtual void OnDimensionChange();

public:

	void SetCornerLength(float _lengthPx);
	void SetSpriteInformation(const SpriteInformation* _siArray);
	virtual void PostAttachment(WidgetWindowData _data) {}
	virtual void DoAction() {}

	virtual void AppendWidgetRenderDataToArrays(
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		std::vector<SpriteInformation>& OUT_batchSpriteInformation,
		float _xOffset,
		float _yOffset
	);

	virtual void AppendWidgetTextDataToArray(
		std::vector<Text>& OUT_textArraysToRender
	) {}

	virtual ~Pane() {}
};







