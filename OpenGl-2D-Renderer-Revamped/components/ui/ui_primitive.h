#pragma once



#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../batch_types/batch_instance_primitives.h"



/*

Abstract class for all graphical primitives.

*/


struct UI_Primitive {

private:

	static void* FUTURE_POINTER_TO_STRING_MANAGER_AND_ALLOCATOR;

protected:

	glm::vec2 m_Dimensions;


	//	Drawcalls will be made to the upper left corner of the window object
	//	therefore primitives require offset from that point.
	glm::vec2 m_PositionRelativeToWindow;

public:

	UI_Primitive(
		glm::vec2 _dimensions,
		glm::vec2 _relativeToWindow
	);


	

};




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

	float m_PositionPairArray[18] = { 0 };
	float m_DimensionPairArray[18] = { 0 };


	uint32_t m_IndexOfSupplementRenderingData = 0;


	float m_CornerSidePx = 20.f;

public:

	Pane(
		glm::vec2 _dimensions
	);


	//	This function outputs vertex coord data for the FreeBatch object.
	//	SpriteInformation data will be added by the UI manager immediately after.
	//	This is made to avoid unnecessary complication while allowing the UI managre to set
	//	custom indices for the corners, sides, and so on.
	//
	//	Output for this function:
	//
	void Record (
		std::vector<float>& OUT_batchPairsOfXYdimensions,
		std::vector<float>& OUT_batchPairsOfXYpositions,
		size_t _indexOfFirstElement
	) const;


	void UpdateArrays();

public:

	void SetIndexOfSupplementRenderingData(uint32_t _index);

};






