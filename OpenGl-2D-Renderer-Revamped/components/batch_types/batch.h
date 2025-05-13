#pragma once


#include <vector>

#include "../../common/common.h"
#include "../drawable.h"

class Batch {

	int m_InstanceCount = -1;

	SpriteSheet* m_SpriteSheet = nullptr;
	Shader* m_Shader = nullptr;

public:

	Batch() {}

	
	Batch(
		Shader* _shader,
		SpriteSheet* _spriteSheet,
		int _instanceCount
	);


	virtual ~Batch();

public:

	int GetInstanceCount() { return m_InstanceCount; }
	Shader* GetShader() { return m_Shader; }
	SpriteSheet* GetSpriteSheet() { return m_SpriteSheet; }

};

