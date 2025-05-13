#include "strict_batch.h"
#include "../../common/standard_quad.h"



StrictBatch::StrictBatch(
	SpriteSheet* _spriteSheet,
	int _instanceCount
)
	: m_SpriteSheet(_spriteSheet),
	m_InstanceCount(_instanceCount)
{
	glGenVertexArrays(1, &m_LocalBatchVAO);
	glGenBuffers(1, &m_SpriteIndexVBO);
}


void StrictBatch::InitialiseBuffers(
	int* _spriteIndexArray
) {
	glBindVertexArray(m_LocalBatchVAO);

	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.m_VextexBuffer);
	glVertexAttribPointer(0, 2, GL_UNSIGNED_INT, GL_FALSE, 2 * sizeof(unsigned int), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_StandardQuad.m_IndexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.m_TexCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_SpriteIndexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * GetInstanceCount(), _spriteIndexArray, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(int), 0);
	glEnableVertexAttribArray(2);


	glVertexAttribDivisor(2, 1);


	glBindVertexArray(0);
}


void StrictBatch::UpdateBuffer(
	int* _spriteIndexArray
) {
	glBindBuffer(GL_ARRAY_BUFFER, m_SpriteIndexVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * GetInstanceCount(), _spriteIndexArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void StrictBatch::Bind() const {
	glBindVertexArray(m_LocalBatchVAO);
}


void StrictBatch::Unbind() const {
	glBindVertexArray(0);
}

