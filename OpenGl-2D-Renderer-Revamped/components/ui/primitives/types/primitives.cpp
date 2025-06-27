#include "../ui_primitive.h"
#include "pane.h"

//		--		UI	PRIMITIVE		--		//


void UI_Primitive::SetDimensions(const glm::vec2& _dimensions){
	DEBUG_ASSERT(_dimensions.x > 0 || _dimensions.y > 0, "Non-positive dimensions passed to Pane object.");
	m_Dimensions = _dimensions;
	OnDimensionChange();
}

void UI_Primitive::SetPositionRelativeToWindow(const glm::vec2& _posRelativeToWindow){
	m_PositionRelativeToWindow = _posRelativeToWindow;
}


//		--			PANE			--		//


Pane::Pane(
	glm::vec2 _dimensions,
	float _cornerLengthPx
) :
	UI_Primitive(_dimensions, glm::vec2(0.f, 0.f)),
	m_CornerSidePx(_cornerLengthPx)
{
	UpdateArrays();
}


void Pane::AppendPaneBatchingData(
	std::vector<float>& OUT_batchPairsOfXYdimensions,
	std::vector<float>& OUT_batchPairsOfXYpositions,
	std::vector<SpriteInformation>& OUT_batchSpriteInformation,
	float _xOffset,
	float _yOffset
) const {
	for (size_t i = 0; i < 9; i++) {
		float xComponent = _xOffset;
		float yComponent = _yOffset;

		xComponent += m_PositionPairArray[i * 2];
		yComponent += m_PositionPairArray[i * 2 + 1];

		OUT_batchPairsOfXYpositions.emplace_back(xComponent);
		OUT_batchPairsOfXYpositions.emplace_back(yComponent);

		OUT_batchPairsOfXYdimensions.emplace_back(m_DimensionPairArray[i * 2]);
		OUT_batchPairsOfXYdimensions.emplace_back(m_DimensionPairArray[i * 2 + 1]);

		OUT_batchSpriteInformation.emplace_back(m_Skin[i]);
	}
}


void Pane::UpdateArrays() {
	float CornerLength, UpperLength, SideLength;

	CornerLength = m_CornerSidePx;
	UpperLength = m_Dimensions.x - CornerLength * 2;
	SideLength = m_Dimensions.y - CornerLength * 2;

	float LongUpperLength = CornerLength + UpperLength;
	float LongSideLength = CornerLength + SideLength;

	auto WriteQuad = [&](size_t _indexOfPair, float u0, float v0, float u1, float v1) {

		size_t VertexIndex, DimensionIndex;
		VertexIndex = _indexOfPair * 2;
		DimensionIndex = _indexOfPair * 2;

		m_PositionPairArray[VertexIndex] = u0;
		m_PositionPairArray[VertexIndex + 1] = v0;

		m_DimensionPairArray[DimensionIndex] = u1;
		m_DimensionPairArray[DimensionIndex + 1] = v1;
	};


	WriteQuad(0, 0, 0, CornerLength, CornerLength);
	WriteQuad(1, LongUpperLength, 0, CornerLength, CornerLength);
	WriteQuad(2, 0, LongSideLength, CornerLength, CornerLength);
	WriteQuad(3, LongUpperLength, LongSideLength, CornerLength, CornerLength);

	WriteQuad(4, CornerLength, 0, LongUpperLength, CornerLength);
	WriteQuad(5, CornerLength, LongSideLength, LongUpperLength, CornerLength);

	WriteQuad(6, 0, CornerLength, CornerLength, LongSideLength);
	WriteQuad(7, LongUpperLength, CornerLength, CornerLength, LongSideLength);

	WriteQuad(8, CornerLength, CornerLength, UpperLength, SideLength);
}


void Pane::SetCornerLength(float _lengthPx) {

	DEBUG_ASSERT(_lengthPx > 1.f, "Strange length passed for Pane corner size.");
	if (_lengthPx < 1.f) return;

	m_CornerSidePx = _lengthPx;
	UpdateArrays();
}


void Pane::SetSpriteInformation(const SpriteInformation* _siArray) {

	DEBUG_ASSERT(_siArray, "Pane was passed nullptr for SpriteInformation array.");
	if (!_siArray) return;

	m_Skin = _siArray;
}


void Pane::OnDimensionChange() {
	UpdateArrays();
}



