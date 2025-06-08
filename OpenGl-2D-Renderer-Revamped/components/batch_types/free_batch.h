#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include "base_batch.h"

/*
TODO:
This batch rendering technique will be repurposed use custom quad coord 
vertices per instance. It'd be very useful for future UI developments.
*/


class FreeBatch : public BaseBatch {

	unsigned int m_VertexVBO = 0;
	unsigned int m_UVRegionVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;

public:

	FreeBatch(
		int _instanceCount
	);


	virtual void InitialiseBuffers();


	virtual void DeleteBuffers();


	void UpdateBuffers(
		const int* _spriteIndices,
		const float* _rotationsArray,
		const float* _pairsOfXYpositions,
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

	unsigned int GetVertexVBO() const { return m_VertexVBO; } 
	unsigned int GetUVRegionVBO() const { return m_UVRegionVBO; }
	unsigned int GetRotationsVBO() const { return m_RotationsVBO; }
	unsigned int GetPositionsVBO() const { return m_PositionsVBO; }


	int GetElementCount() const { return m_InstanceCount * 6; }

	virtual ~FreeBatch() {}
};


