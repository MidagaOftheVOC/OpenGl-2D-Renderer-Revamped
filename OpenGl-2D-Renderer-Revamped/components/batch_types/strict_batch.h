#pragma once


#include "../../common/common.h"

#include "base_batch.h"

/*
*	StrictBatch:	A Batch for instanced rendering where all objects have
*					strong resrtictions.
*
*	Requirements for drawn objects:
*		-	placed in a grid, relative to each other
*		-	they're not rotated
*		-	have uniform scale
*		-	only need world space position to draw
*/

class StrictBatch : public BaseBatch {

	unsigned int m_UVRegionVBO = 0;
	int m_SpritesPerRow = 0;

public:

	StrictBatch(
		const SpriteSheet* _spriteSheet,
		int _instanceCount,
		int _spriteCountPerRow
	);

	
	virtual void InitialiseBuffers();


	virtual void DeleteBuffers();


	void SetSpriteCountPerRow(
		int _spriteCountPerRow
	);


	void UpdateBuffer(
		const int* _spriteIndexArray,
		const size_t _arrayElementCount
	);

	
	void BindUniqueBuffers() const;
	
private:

	static unsigned int s_VAO;

public:

	static void InitialiseCommonVAO();


	static void BindCommonVAO();


	static void UnbindCommonVAO();

public:

	int GetSpriteCountPerRow() const { return m_SpritesPerRow; }
	unsigned int GetSpriteIndexVBO() const { return m_UVRegionVBO; }
	
	virtual ~StrictBatch() {}
};

