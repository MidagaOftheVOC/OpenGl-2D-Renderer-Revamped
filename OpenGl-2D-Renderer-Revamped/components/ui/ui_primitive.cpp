#include "ui_primitive.h"


//		--		UI PRIMITIVE		--		//

void* UI_Primitive::FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR = nullptr;


UI_Primitive::UI_Primitive(
	glm::vec2 _dimensions,
	glm::vec2 _relativeToWindow
) :
	m_Dimensions(_dimensions),
	m_PositionRelativeToWindow(_relativeToWindow)
{}




//		--			PANE			--		//


Pane::Pane(
	glm::vec2 _dimensions
) :
	UI_Primitive(_dimensions, glm::vec2(0.f, 0.f))
{}


void Pane::Record(
	std::vector<float>& OUT_batchPairsOfXYdimensions,
	std::vector<float>& OUT_batchPairsOfXYpositions,
	size_t _indexOfFirstElement
) const {

	DEBUG_ASSERT((OUT_batchPairsOfXYdimensions.size() - _indexOfFirstElement) >= 18, "Pane::SplitPane(), passed vectors have insufficient memory.");
	DEBUG_ASSERT((OUT_batchPairsOfXYpositions.size() - _indexOfFirstElement) >= 18, "Pane::SplitPane(), passed vectors have insufficient memory.");

	memcpy(OUT_batchPairsOfXYdimensions.data() + _indexOfFirstElement, m_DimensionPairArray, sizeof(float) * 18);
	memcpy(OUT_batchPairsOfXYpositions.data() + _indexOfFirstElement, m_PositionPairArray, sizeof(float) * 18);
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


	WriteQuad(0, 0,					0,					CornerLength,		CornerLength);
	WriteQuad(1, LongUpperLength,	0,					CornerLength,		CornerLength);
	WriteQuad(2, 0,					LongSideLength,		CornerLength,		CornerLength);
	WriteQuad(3, LongUpperLength,	LongSideLength,		CornerLength,		CornerLength);

	WriteQuad(4, CornerLength,		0,					LongUpperLength,	CornerLength);
	WriteQuad(5, CornerLength,		LongSideLength,		LongUpperLength,	CornerLength);

	WriteQuad(6, 0,					CornerLength,		CornerLength,		LongSideLength);
	WriteQuad(7, LongUpperLength,	CornerLength,		CornerLength,		LongSideLength);

	WriteQuad(8, CornerLength,		CornerLength,		UpperLength,		SideLength);
}






