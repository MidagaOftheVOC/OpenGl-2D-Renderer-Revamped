#include "batch.h"


Batch::Batch(
	Shader* _shader,
	SpriteSheet* _spriteSheet,
	int _instanceCount
)
	: m_Shader(_shader),
	m_SpriteSheet(_spriteSheet),
	m_InstanceCount(_instanceCount)
{
}


Batch::~Batch() {
}

