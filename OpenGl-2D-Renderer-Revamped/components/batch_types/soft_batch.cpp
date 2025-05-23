#include "soft_batch.h"


SoftBatch::SoftBatch(
	const SpriteSheet* _spriteSheet,
	int _instanceCount
):
	m_SpriteSheet(_spriteSheet),
	m_InstanceCount(_instanceCount)
{}


void SoftBatch::InitialiseBuffers(
	const int* _spriteIndexArray,
	const float* _objectRotationsRad,
	const float* _pairsOfxyPositions
) {

	glGenVertexArrays(1, &m_LocalBatchVAO);
	glBindVertexArray(m_LocalBatchVAO);

	unsigned int vbo[3];

	glGenBuffers(3, vbo);

	g_StandardQuad.BindVertexBufferAt(0);
	g_StandardQuad.BindTexUVbufferAt(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_StandardQuad.m_IndexBuffer);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);	//	1 int / instance		- sprite index
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * GetInstanceCount(), _spriteIndexArray, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(int), static_cast<void*>(0));
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);	//	1 float / instance		- rotations
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetInstanceCount(), _objectRotationsRad, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), static_cast<void*>(0));
	glVertexAttribDivisor(3, 1);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);	//	2 float / instance		- positions
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 2) * GetInstanceCount(), _pairsOfxyPositions, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, static_cast<void*>(0));
	glVertexAttribDivisor(4, 1);
	

	m_SpriteIndexVBO = vbo[0];
	m_RotationsVBO = vbo[1];
	m_PositionsVBO = vbo[2];

	glBindVertexArray(0);
}


//	TODO: implement

void SoftBatch::UpdateSpriteIndexBuffer(
	const int* _spriteIndexArray
) {
	glBindBuffer(GL_ARRAY_BUFFER, m_SpriteIndexVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * m_InstanceCount, _spriteIndexArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SoftBatch::UpdateRotationsBuffer(
	const float* _objectRotationsRad
) {
	glBindBuffer(GL_ARRAY_BUFFER, m_RotationsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_InstanceCount, _objectRotationsRad);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SoftBatch::UpdatePositionsBuffer(
	const float* _pairsOfxyPositions
) {
	glBindBuffer(GL_ARRAY_BUFFER, m_PositionsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * m_InstanceCount, _pairsOfxyPositions);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SoftBatch::Bind() const {
	glBindVertexArray(m_LocalBatchVAO);
}


void SoftBatch::Unbind() const {
	glBindVertexArray(0);
}



