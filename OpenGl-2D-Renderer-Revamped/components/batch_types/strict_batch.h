#pragma once


#include "../../common/common.h"

#include "../sprite_sheet.h"

/*
*	StrictBatch:	A Batch for instanced rendering where all objects rendered have
*					strong resrtictions.
*
*	Requirements for drawn objects:
*		-	placed in a grid, relative to each other
*		-	they're not rotated
*		-	have uniform scale
*		-	only need world space position to draw
*/

class StrictBatch {

	unsigned int m_SpriteIndexVBO = 0;
	unsigned int m_LocalBatchVAO = 0;

	int m_InstanceCount = -1;
	SpriteSheet* m_SpriteSheet = nullptr;

public:

	StrictBatch(
		SpriteSheet* _spriteSheet,
		int _instanceCount
	);

	
	void InitialiseBuffers(
		int* _spriteIndexArray
	);


	void UpdateBuffer(
		int* _spriteIndexArray
	);

	
	void Bind() const;


	void Unbind() const;

public:

	int GetInstanceCount() const { return m_InstanceCount; }
	SpriteSheet* GetSheet() const { return m_SpriteSheet; }
	unsigned int GetSpriteIndexVBO() const { return m_SpriteIndexVBO; }

};

