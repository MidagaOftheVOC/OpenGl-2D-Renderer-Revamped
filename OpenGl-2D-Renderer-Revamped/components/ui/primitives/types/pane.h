#pragma once


#include "../ui_primitive.h"


/*

Pane:
Graphical primitive which must represetn a rectangle split in 9 segments.

+-+----------+-+
|0|    4     |1|
+-+----------+-+
| |          | |
|6|    8     |7|
| |          | |
+-+----------+-+
|2|    5     |3|
+-+----------+-+

Corners of the pane are squares.
Middle and centrail borders are rectangles.
In total:
-4 unique shapes
-3 unique lengths

*/
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
		float _cornerLengthPx
	);


	//	This function outputs vertex coord data for the FreeBatch object.
	//	SpriteInformation data will be added by the UI manager immediately after.
	//	This is made to avoid unnecessary complication while allowing the UI managre to set
	//	custom indices for the corners, sides, and so on.
	void AppendPaneBatchingData(
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		std::vector<SpriteInformation>& OUT_batchSpriteInformation,
		float _xOffset,
		float _yOffset
	) const;

private:

	void UpdateArrays();


	virtual void OnDimensionChange();

public:

	void SetCornerLength(float _lengthPx);
	void SetSpriteInformation(const SpriteInformation* _siArray);

};







