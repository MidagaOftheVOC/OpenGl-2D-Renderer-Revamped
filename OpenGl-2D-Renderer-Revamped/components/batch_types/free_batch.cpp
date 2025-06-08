#include "free_batch.h"

unsigned int FreeBatch::s_VAO = 0;




FreeBatch::FreeBatch(
	int _instanceCount
) :
	BaseBatch(_instanceCount)
{}


void FreeBatch::InitialiseBuffers() {
	unsigned int vbo[6];
	glCreateBuffers(6, vbo);

	m_SpriteInformationVBO = vbo[0];
	m_RotationsVBO = vbo[1];
	m_PositionsVBO = vbo[2];
	m_VertexDimensionsVBO = vbo[3];

	m_SheetUVRegionsUBO = vbo[4];
	m_SheetIndexOffsetsUBO = vbo[5];
}


void FreeBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_SpriteInformationVBO);
	glDeleteBuffers(1, &m_RotationsVBO);
	glDeleteBuffers(1, &m_PositionsVBO);
	glDeleteBuffers(1, &m_VertexDimensionsVBO);

	glDeleteBuffers(1, &m_SheetUVRegionsUBO);
	glDeleteBuffers(1, &m_SheetIndexOffsetsUBO);
}


void FreeBatch::InitialiseCommonVAO() {


	if (GLdiagnostics::IsVertexArray(s_VAO)) {
		DEBUG_ASSERT(0, "Attempt to reinitialise common FreeBatch VAO object.");
		return;
	}


	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);


	//	@0 - common quad VBO
	g_StandardQuad.BindVertexBufferAt(0);

	//	@element - common element VBO
	g_StandardQuad.BindElementIndexArray();
	
	//	@1 - common texture VBO
	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.GetUnmodifiedTextureUVBuffer());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(1);
	
	//	@2 1* SpriteInformation / instance - sheet/sprite indices for UV region mapping
	glBindVertexBuffer(2, 0, 0, 1 * sizeof(SpriteInformation));
	glVertexAttribIFormat(2, 1, GL_UNSIGNED_SHORT, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribBinding(2, 2);
	glVertexAttribDivisor(2, 1);
	
	//	@3 1* float  / instance - object rotations
	glBindVertexBuffer(3, 0, 0, 1 * sizeof(float));
	glVertexAttribFormat(3, 1, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribBinding(3, 3);
	glVertexAttribDivisor(3, 1);
	
	//	@4 2* float  / instance - pais of XY offset positions relative to batch draw call position
	glBindVertexBuffer(4, 0, 0, 2 * sizeof(float));
	glVertexAttribFormat(4, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribBinding(4, 4);
	glVertexAttribDivisor(4, 1);

	//	@5 2* float  / instance - multiply non-zero X and Y components of standard quad VBO to get custom sizes
	glBindVertexBuffer(5, 0, 0, 2 * sizeof(float));
	glVertexAttribFormat(5, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(5);
	glVertexAttribBinding(5, 5);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
}


void FreeBatch::UpdateBuffers(
	const SpriteInformation* _spriteInformation,
	const float* _rotationsArray,
	const float* _pairsOfXYpositions,
	const float* _pairsOfXYdimensions,
	const size_t _arrayElementCount
) {
	SetInstanceCount(static_cast<int>(_arrayElementCount));
	
	if (m_Flags.CheckAndClearFlag(c_MaximumInstanceCountExceeded)) {

		m_Flags.ClearFlag(c_NotInitialised);
		
		glNamedBufferData(	
			m_SpriteInformationVBO, sizeof(SpriteInformation) * GetInstanceCount(),	_spriteInformation,		GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_RotationsVBO,			sizeof(float) * GetInstanceCount(),				_rotationsArray,		GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_PositionsVBO,			sizeof(float) * 2 * GetInstanceCount(),			_pairsOfXYpositions,	GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_VertexDimensionsVBO,  sizeof(float) * 2 * GetInstanceCount(),			_pairsOfXYdimensions,	GL_DYNAMIC_DRAW );

		return;
	}

	if (_spriteInformation)		
		glNamedBufferSubData(m_SpriteInformationVBO, 0, sizeof(SpriteInformation) * GetInstanceCount(), _spriteInformation );

	if (_rotationsArray)		
		glNamedBufferSubData(m_RotationsVBO, 0,			sizeof(float) * GetInstanceCount(),				_rotationsArray);

	if (_pairsOfXYpositions)	
		glNamedBufferSubData(m_PositionsVBO, 0,			sizeof(float) * 2 * GetInstanceCount(),			_pairsOfXYpositions);

	if (_pairsOfXYdimensions)	
		glNamedBufferSubData(m_VertexDimensionsVBO, 0,	sizeof(float) * 2 * GetInstanceCount(),			_pairsOfXYdimensions);
}


void FreeBatch::BindUniqueBuffers() const {
	glBindVertexBuffer(2, m_SpriteInformationVBO, 0, sizeof(SpriteInformation));
	glBindVertexBuffer(3, m_RotationsVBO, 0, sizeof(float));
	glBindVertexBuffer(4, m_PositionsVBO, 0, sizeof(float) * 2);
	glBindVertexBuffer(5, m_VertexDimensionsVBO, 0, sizeof(float) * 2);
}


void FreeBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void FreeBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}
