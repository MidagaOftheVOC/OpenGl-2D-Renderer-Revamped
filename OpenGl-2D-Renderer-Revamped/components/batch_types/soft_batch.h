#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

/*
*	SoftBatch:	A batch for rendering many objects with looser 
*				restrictions than the StrictBatch
* 
*	Supports per-instance:
*		- sprite index
*		- rotation
*		- position
*/

class SoftBatch {

	unsigned int m_LocalBatchVAO = 0;

	unsigned int m_SpriteIndexVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;

	const SpriteSheet* m_SpriteSheet = nullptr;

	int m_InstanceCount = 0;

public:

	SoftBatch(
		const SpriteSheet* _spriteSheet,
		int _instanceCount
	);


	void InitialiseBuffers(
		const int* _spriteIndexArray,
		const float* _objectRotationsRad,
		const float* _pairsOfxyPositions
	);


	void UpdateSpriteIndexBuffer(
		const int* _spriteIndexArray
	);


	void UpdateRotationsBuffer(
		const float* _objectRotationsRad
	);


	void UpdatePositionsBuffer(
		const float* _pairsOfxyPositions
	);


	void Bind() const;


	void Unbind() const;

public:

	int GetInstanceCount() const { return m_InstanceCount; }
	const SpriteSheet* GetSheet() const { return m_SpriteSheet; }
	unsigned int GetSpriteIndexVBO() const { return m_SpriteIndexVBO; }

};

