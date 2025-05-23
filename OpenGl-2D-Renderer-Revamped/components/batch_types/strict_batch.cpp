#include "strict_batch.h"
#include "../../common/standard_quad.h"



StrictBatch::StrictBatch(
	SpriteSheet* _spriteSheet,
	int _instanceCount
)
	: m_SpriteSheet(_spriteSheet),
	m_InstanceCount(_instanceCount)
{
	glCreateBuffers(1, &m_SpriteIndexVBO);
}


unsigned int StrictBatch::s_VAO = 0;


void StrictBatch::InitialiseCommonVAO() {

	bool IsVertex = glIsVertexArray(s_VAO) == GL_TRUE;

	if (IsVertex) {
		DEBUG_ASSERT(0, "Reinitialising common VAO for class StrictBatch.");
		return;
	}

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	g_StandardQuad.BindVertexBufferAt(0);
	g_StandardQuad.BindTexUVbufferAt(1);
	g_StandardQuad.BindElementIndexArray();

	glBindVertexBuffer(2, 0, 0, 1 * sizeof(unsigned int));
	glEnableVertexAttribArray(2);
	glVertexAttribIFormat(2, 1, GL_UNSIGNED_INT, 0);
	glVertexAttribBinding(2, 2);
	glVertexBindingDivisor(2, 1);

	glBindVertexArray(0);
}


void StrictBatch::InitialiseBuffers(
	int* _spriteIndexArray
) {
	glNamedBufferData(m_SpriteIndexVBO, sizeof(unsigned int) * GetInstanceCount(), _spriteIndexArray, GL_DYNAMIC_DRAW);
}


void StrictBatch::UpdateBuffer(
	int* _spriteIndexArray
) {
	glNamedBufferSubData(m_SpriteIndexVBO, 0, sizeof(unsigned int) * GetInstanceCount(), _spriteIndexArray);
}


void StrictBatch::BindUniqueBuffers() const {
	glVertexArrayVertexBuffer(s_VAO, 2, m_SpriteIndexVBO, 0, sizeof(unsigned int) * 1);
}


void StrictBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void StrictBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}

