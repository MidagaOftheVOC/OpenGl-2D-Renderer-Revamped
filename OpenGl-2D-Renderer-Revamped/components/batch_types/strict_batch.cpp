#include "strict_batch.h"
#include "../../common/standard_quad.h"



StrictBatch::StrictBatch(
	SpriteSheet* _spriteSheet,
	int _instanceCount
)
	: m_SpriteSheet(_spriteSheet),
	m_InstanceCount(_instanceCount)
{
	glCreateBuffers(1, &m_UVRegionVBO);
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
	g_StandardQuad.BindElementIndexArray();

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.GetUnmodifiedTextureUVBuffer());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);



	glBindVertexBuffer(2, 0, 0, 4 * sizeof(float));
	glEnableVertexAttribArray(2);
	glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);
	glVertexBindingDivisor(2, 1);

	glBindVertexArray(0);
}


void StrictBatch::InitialiseBuffers(
	int* _spriteIndexArray
) {
	std::vector<UVRegion> UVRegionArray;
	GetSheet()->TransformIndicesToUVRegionArray(_spriteIndexArray, GetInstanceCount(), UVRegionArray);
	glNamedBufferData(m_UVRegionVBO, sizeof(float) * 4 * GetInstanceCount(), UVRegionArray.data(), GL_DYNAMIC_DRAW);
}


void StrictBatch::UpdateBuffer(
	int* _spriteIndexArray
) {
	std::vector<UVRegion> UVRegionArray;
	GetSheet()->TransformIndicesToUVRegionArray(_spriteIndexArray, GetInstanceCount(), UVRegionArray);
	glNamedBufferSubData(m_UVRegionVBO, 0, sizeof(float) * 4 * GetInstanceCount(), UVRegionArray.data());
}


void StrictBatch::BindUniqueBuffers() const {
	glVertexArrayVertexBuffer(s_VAO, 2, m_UVRegionVBO, 0, sizeof(float) * 4);
}


void StrictBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void StrictBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}

