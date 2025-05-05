#pragma once


#include <vector>

#include "../common/common.h"
#include "drawable.h"

class Batch {

	//	Updated each frame.
	//	Will be initiated with GL_DYNAMIC
	unsigned int m_UVtextureVertexBufferID = 0;

	//	Updated each frame.
	//	Will be initiated with GL_DYNAMIC
	unsigned int m_MatrixVertexBuffer = 0;


	int m_InstanceCount = -1;

	
	bool m_IsUVtextureBuffered = false;
	bool m_IsMatricesBuffered = false;


	SpriteSheet* m_SpriteSheet = nullptr;
	Shader* m_Shader = nullptr;

public:

	Batch() {}


	Batch(
		Shader* _shader,
		SpriteSheet* _spriteSheet,
		int _instanceCount
	);


	void BufferUVtextureData(
		const std::vector< UVRegion > &_uvRegionArray
	);

	void BufferMatrixData(
		const std::vector< glm::mat4 > &_matrixArray
	);


	void GetBuffersForInstancedRendering(
		unsigned int& OUT_UVtextureVertexBuffer,
		unsigned int& OUT_MatrixVertexBuffer
	);

	int GetInstanceCount() { return m_InstanceCount; }
	Shader* GetShader() { return m_Shader; }
	SpriteSheet* GetSpriteSheet() { return m_SpriteSheet; }

	~Batch();

private:

	bool IsTextureArrayBuffered() { return m_IsUVtextureBuffered; }
	bool isMatrixArrayBuffered() { return m_IsMatricesBuffered; }
};

