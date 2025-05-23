#pragma once


#include "../../common/common.h"

#include "../sprite_sheet.h"

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

class StrictBatch {

	unsigned int m_SpriteIndexVBO = 0;
	unsigned int m_LocalBatchVAO = 0;

	int m_InstanceCount = -1;
	const SpriteSheet* m_SpriteSheet = nullptr;

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

	
	void BindUniqueBuffers() const;

#ifdef DEBUG__CODE

	std::string d_Name;
	void DEBUG_SetName(const std::string& _name) { d_Name = _name; }

#endif
	
private:

	static unsigned int s_VAO;

public:

	static void InitialiseCommonVAO();


	static void BindCommonVAO();


	static void UnbindCommonVAO();

public:

	int GetInstanceCount() const { return m_InstanceCount; }
	const SpriteSheet* GetSheet() const { return m_SpriteSheet; }
	unsigned int GetSpriteIndexVBO() const { return m_SpriteIndexVBO; }

};

