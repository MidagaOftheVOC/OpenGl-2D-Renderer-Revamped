#include "soft_batch.h"


unsigned int SoftBatch::s_VAO = 0;


SoftBatch::SoftBatch(
	const SpriteSheet* _spriteSheet,
	int _instanceCount,
	SoftBatchType _type
):
	BaseBatch(_spriteSheet, _instanceCount),
	m_Type(_type)
{}


void SoftBatch::InitialiseBuffers() {
	unsigned int vbo[3];
	glCreateBuffers(3, vbo);

	m_UVRegionsVBO = vbo[0];
	m_RotationsVBO = vbo[1];
	m_PositionsVBO = vbo[2];
}


void SoftBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_UVRegionsVBO);
	glDeleteBuffers(1, &m_RotationsVBO);
	glDeleteBuffers(1, &m_PositionsVBO);
}


void SoftBatch::InitialiseCommonVAO() {
	
	if (GLdiagnostics::IsVertexArray(s_VAO)) {
		DEBUG_ASSERT(0, "Attempt to reinitialise common SoftBatch VAO object.");
		return;
	}
	
	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);
	

	g_StandardQuad.BindVertexBufferAt(0);

	//	Unmodified Coordinate vertices for square quad.
	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.GetUnmodifiedTextureUVBuffer());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_StandardQuad.m_IndexBuffer);

	//	UV regions for texture UVs
	//	@2	4* floats / instance
	glBindVertexBuffer(2, 0, 0, 4 * sizeof(float));
	glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribBinding(2, 2);
	glVertexBindingDivisor(2, 1);

	//	Floats for radians, rotations per sprite
	//	@3	1* float / instance
	glBindVertexBuffer(3, 0, 0, 1 * sizeof(float));
	glVertexAttribFormat(3, 1, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribBinding(3, 3);
	glVertexBindingDivisor(3, 1);

	//	Pairs of floats for position offsets
	//	@4 2* floats / instance
	glBindVertexBuffer(4, 0, 0, 2 * sizeof(float));
	glVertexAttribFormat(4, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribBinding(4, 4);
	glVertexBindingDivisor(4, 1);


	glBindVertexArray(0);
}


void SoftBatch::UpdateBuffers(
	const int* _spriteIndices,
	const float* _objectRotationsRad,
	const float* _pairsOfxyPositions,
	const size_t _arrayElementCount
) {
	SetInstanceCount(static_cast<unsigned int>(_arrayElementCount));
	
	std::vector<UVRegion> UVs;
	UVRegion* UVArray = nullptr;

	if (_spriteIndices) {
		GetSheet()->TransformIndicesToUVRegionArray(_spriteIndices, GetInstanceCount(), UVs);
		UVArray = UVs.data();
	}
	
	//	Update all, even with null data.
	if ( m_Flags.CheckFlag(c_MaximumInstanceCountExceeded)) {

		m_Flags.ClearFlag(c_MaximumInstanceCountExceeded);

		glNamedBufferData(GetUVRegionsVBO(), GetInstanceCount() * sizeof(UVRegion), UVArray, GL_DYNAMIC_DRAW);
		glNamedBufferData(GetRotationsVBO(), GetInstanceCount() * sizeof(float), _objectRotationsRad, GL_DYNAMIC_DRAW);
		glNamedBufferData(GetPositionsVBO(), GetInstanceCount() * 2 * sizeof(float), _pairsOfxyPositions, GL_DYNAMIC_DRAW);

		return;
	}

	if(UVArray)				glNamedBufferSubData(GetUVRegionsVBO(), 0, GetInstanceCount() * sizeof(UVRegion), UVArray);
	if(_objectRotationsRad)	glNamedBufferSubData(GetRotationsVBO(), 0, GetInstanceCount() * sizeof(float), _objectRotationsRad);
	if(_pairsOfxyPositions)	glNamedBufferSubData(GetPositionsVBO(), 0, GetInstanceCount() * 2 * sizeof(float), _pairsOfxyPositions);
}


bool SoftBatch::UpdateUVRegionVBO(
	const int* _spriteIndices,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());
	
	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _spriteIndices == nullptr || _arrayElementCount >= GetInstanceCount()) {
		return false;
	}

	std::vector<UVRegion> UVs;
	GetSheet()->TransformIndicesToUVRegionArray(_spriteIndices, GetInstanceCount(), UVs);

	glNamedBufferSubData(GetUVRegionsVBO(), 0, GetInstanceCount() * sizeof(UVRegion), UVs.data());
	return true;
}


bool SoftBatch::UpdateRotationsBuffer(
	const float* _objectRotationsRad,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());

	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _objectRotationsRad == nullptr || _arrayElementCount >= GetInstanceCount()) {
		return false;
	}

	glNamedBufferSubData(GetRotationsVBO(), 0, GetInstanceCount() * 1 * sizeof(float), _objectRotationsRad);
	return true;
}


bool SoftBatch::UpdatePositionsBuffer(
	const float* _pairsOfxyPositions,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());

	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _pairsOfxyPositions == nullptr || _arrayElementCount >= GetInstanceCount()) {
		return false;
	}

	glNamedBufferSubData(GetPositionsVBO(), 0, GetInstanceCount() * 2 * sizeof(float), _pairsOfxyPositions);
	return true;
}


void SoftBatch::BindUniqueBuffers() const {
	glBindVertexBuffer(2, GetUVRegionsVBO(), 0, sizeof(UVRegion));
	glBindVertexBuffer(3, GetRotationsVBO(), 0, sizeof(float));
	glBindVertexBuffer(4, GetPositionsVBO(), 0, sizeof(float) * 2);
}


void SoftBatch::BindCommonVAO(){
	glBindVertexArray(s_VAO);
}


void SoftBatch::UnbindCommonVAO(){
	glBindVertexArray(0);
}
