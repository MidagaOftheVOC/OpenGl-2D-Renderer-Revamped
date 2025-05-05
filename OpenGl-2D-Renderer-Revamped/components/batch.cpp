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
	glGenBuffers(1, &m_UVtextureVertexBufferID);
	glGenBuffers(1, &m_MatrixVertexBuffer);
}


void Batch::BufferUVtextureData(
	const std::vector< UVRegion > &_uvRegionArray
) {

	DEBUG_ASSERT(GetInstanceCount() == _uvRegionArray.size(),
		"Data sent to Batch object has wrong instance count.");

	glBindBuffer(GL_ARRAY_BUFFER, m_UVtextureVertexBufferID);
	if (!IsTextureArrayBuffered()) {
		glBufferData(
			GL_ARRAY_BUFFER,
			_uvRegionArray.size() * sizeof(UVRegion),
			_uvRegionArray.data(),
			GL_DYNAMIC_DRAW
		);
		m_IsUVtextureBuffered = true;
	}
	else {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			_uvRegionArray.size() * sizeof(UVRegion),
			_uvRegionArray.data()
		);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Batch::BufferMatrixData(
	const std::vector< glm::mat4 > &_matrixArray
) {
	DEBUG_ASSERT(GetInstanceCount() == _matrixArray.size(),
		"Data sent to Batch object has wrong instance count.");

	glBindBuffer(GL_ARRAY_BUFFER, m_MatrixVertexBuffer);
	if (!isMatrixArrayBuffered()) {
		glBufferData(
			GL_ARRAY_BUFFER,
			_matrixArray.size() * sizeof(glm::mat4),
			_matrixArray.data(),
			GL_DYNAMIC_DRAW
		);
		m_IsMatricesBuffered = true;
	}
	else {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			_matrixArray.size() * sizeof(glm::mat4),
			_matrixArray.data()
		);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Batch::~Batch() {
	if (m_MatrixVertexBuffer != 0)
		glDeleteBuffers(1, &m_MatrixVertexBuffer);

	if (m_UVtextureVertexBufferID != 0)
		glDeleteBuffers(1, &m_UVtextureVertexBufferID);
}


void Batch::GetBuffersForInstancedRendering(
	unsigned int& OUT_UVtextureVertexBuffer,
	unsigned int& OUT_MatrixVertexBuffer
) {
	OUT_UVtextureVertexBuffer = m_UVtextureVertexBufferID;
	OUT_MatrixVertexBuffer = m_MatrixVertexBuffer;
}


