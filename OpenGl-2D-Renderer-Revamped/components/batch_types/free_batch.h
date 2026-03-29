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

//	Prototype for now
struct Sprite {
	std::string SheetName;
	//	 OR
	size_t SheetIndex;


	size_t IndexWithinSheet;
	
	glm::vec2 Dims;

	float Rotation = 0.f;
};


class FreeBatch : public BaseBatch {

	unsigned int m_VertexDimensionsVBO = 0;
	unsigned int m_SpriteInformationVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;

public:

	FreeBatch(
		int _instanceCount
	);


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


	int GetElementCount() const { return m_InstanceCount * 6; }

	virtual ~FreeBatch() {}
};


