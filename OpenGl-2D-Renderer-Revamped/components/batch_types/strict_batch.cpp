#include "strict_batch.h"
#include "../../common/standard_quad.h"


unsigned int StrictBatch::s_VAO = 0;


StrictBatch::StrictBatch(
	SpriteSheet* _spriteSheet,
	int _instanceCount,
	int _spriteCountPerRow
): 
	BaseBatch(_spriteSheet, _instanceCount),
	m_SpritesPerRow(_spriteCountPerRow)
{}


void StrictBatch::InitialiseBuffers() {
	glCreateBuffers(1, &m_UVRegionVBO);
}


void StrictBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_UVRegionVBO);
}


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


void StrictBatch::SetSpriteCountPerRow(
	int _spriteCountPerRow
) {
	DEBUG_ASSERT(_spriteCountPerRow > 0, "Sprite count set to 0 or lower for StrictBatch with name [%s].", dm_BatchName.c_str());
	m_SpritesPerRow = _spriteCountPerRow;
}


void StrictBatch::UpdateBuffer(
	const int* _spriteIndexArray,
	const size_t _arrayElementCount
) {
	SetInstanceCount(static_cast<int>(_arrayElementCount));
	
	std::vector<UVRegion> UVRegionArray;
	GetSheet()->TransformIndicesToUVRegionArray(_spriteIndexArray, GetInstanceCount(), UVRegionArray);

	if (m_Flags.CheckAndClearFlag(c_MaximumInstanceCountExceeded)) {

		glNamedBufferData(m_UVRegionVBO, sizeof(UVRegion) * GetInstanceCount(), UVRegionArray.data(), GL_DYNAMIC_DRAW);

		return;
	}

	glNamedBufferSubData(m_UVRegionVBO, 0, sizeof(UVRegion) * GetInstanceCount(), UVRegionArray.data());
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

