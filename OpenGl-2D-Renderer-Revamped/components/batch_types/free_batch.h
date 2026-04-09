#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include "base_batch.h"
#include "batch_instance_primitives.h"

/*
This batch rendering technique will use custom quad coord 
vertices per instance. 

What must be improved is the addition of a type, which can be 'added'/'appended' to this batch's arrays to make the user experience better.
This must be incorporated at the base batch level and further extended to each batch function later.
For now, this will work only for FreeBatch since it's the most flexible of all types of batches.
*/

class FreeBatch : public BaseBatch {

	unsigned int m_VertexDimensionsVBO = 0;
	unsigned int m_SpriteInformationVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;

	std::vector<SpriteInformation> m_SIArray;
	std::vector<xyPosition> m_Positions;
	std::vector<float> m_Rotations;
	std::vector<SpriteDimensions> m_Dimensions;

public:

	FreeBatch();

	FreeBatch(
		int _instanceCount
	);


	//	TODO AFTER REFACTOR:
	//	THIS MUST BE COMMON LOGIC, IT MAKES NO SENSE FOR THIS TO BE FREEBATCH-SPECIFIC
	virtual void DrawSpriteInstance(
		const SpriteInstance& spriteInstance,
		float x,
		float y,
		float rotation
	);

	virtual int SendSpriteDataToGPU();


	virtual void InitialiseBuffers();


	virtual void DeleteBuffers();


	void UpdateBuffers(
		const SpriteInformation* _spriteInformation,
		const float* _rotationsArray,
		const float* _pairsOfXYpositions,
		const float* _pairsOfXYdimensions,
		const size_t _arrayElementCount = 0
	);


	void UpdateVertexBuffer(
		const float* _vertexCoordArray,
		const size_t _araryElementCount
	);


	void UpdateUVRegionBuffer(
		const UVRegion* _uvRegionArray,
		const size_t _araryElementCount
	);


	void UpdateRotationsBuffer(
		const float* _rotationsArray,
		const size_t _araryElementCount
	);

	
	void UpdatePositionsBuffer(
		const float* _pairsOfXYpositions,
		const size_t _araryElementCount
	);


	void BindUniqueBuffers() const;

private:

	static unsigned int s_VAO;

public:

	static void InitialiseCommonVAO();


	static void BindCommonVAO();


	static void UnbindCommonVAO();

public:

	unsigned int GetVertexDimensionsVBO() const { return m_VertexDimensionsVBO; }
	unsigned int GetUVRegionVBO() const { return m_SpriteInformationVBO; }
	unsigned int GetRotationsVBO() const { return m_RotationsVBO; }
	unsigned int GetPositionsVBO() const { return m_PositionsVBO; }


	//	TODO: wtf?
	int GetElementCount() const { return m_MaxInstanceCapacity * 6; }

	virtual ~FreeBatch() {}
};


