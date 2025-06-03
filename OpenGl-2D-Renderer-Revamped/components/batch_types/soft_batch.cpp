#include "soft_batch.h"


unsigned int SoftBatch::s_VAO = 0;


SoftBatch::SoftBatch(
	int _instanceCount,
	SoftBatchType _type
):
	BaseBatch(_instanceCount),
	m_Type(_type)
{}


void SoftBatch::InitialiseBuffers() {
	unsigned int vbo[5];
	glCreateBuffers(5, vbo);

	m_SpriteIndexVBO = vbo[0];
	m_RotationsVBO = vbo[1];
	m_PositionsVBO = vbo[2];

	m_SheetUVRegionsUBO = vbo[3];
	m_SheetIndexOffsetsUBO = vbo[4];

	BufferUBOs();
}


void SoftBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_SpriteIndexVBO);
	glDeleteBuffers(1, &m_RotationsVBO);
	glDeleteBuffers(1, &m_PositionsVBO);
	glDeleteBuffers(1, &m_SheetUVRegionsUBO);
}


void SoftBatch::BufferUBOs() {
	glBindBuffer(GL_UNIFORM_BUFFER, m_SheetUVRegionsUBO);

	std::vector<UVRegion> UVs;
	for (const auto& sheet : m_SpriteSheets) {
		const auto& arr = sheet->GetUVRegionArray();
		UVs.insert(UVs.end(), arr.begin(), arr.end());
	}

	glBufferData(GL_UNIFORM_BUFFER, UVs.size() * sizeof(UVRegion), UVs.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_UNIFORM_BUFFER, m_SheetIndexOffsetsUBO);
	std::vector<int> Offsets;
	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		Offsets.emplace_back(static_cast<int>(m_SpriteSheets[i]->GetContainedSpriteCount()));
	}

	glBufferData(GL_UNIFORM_BUFFER, Offsets.size() * sizeof(int), Offsets.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void SoftBatch::BindUBOs() const {
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_SheetUVRegionsUBO);       
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_SheetIndexOffsetsUBO);    
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
	//	@2	1* unsigned short / instance
	glBindVertexBuffer(2, 0, 0, 1 * sizeof(unsigned short));
	glVertexAttribIFormat(2, 1, GL_UNSIGNED_SHORT, 0);
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
	const unsigned short* _spriteIndices,
	const float* _objectRotationsRad,
	const float* _pairsOfxyPositions,
	const size_t _arrayElementCount
) {
	SetInstanceCount(static_cast<unsigned int>(_arrayElementCount));
	
	/*std::vector<UVRegion> UVs;
	UVRegion* UVArray = nullptr;

	if (_spriteIndices) {
		GetSpecialSheetPointer()->TransformIndicesToUVRegionArray(_spriteIndices, GetInstanceCount(), UVs);
		UVArray = UVs.data();
	}*/
	
	//	Update all, even with null data.
	if ( m_Flags.CheckFlag(c_MaximumInstanceCountExceeded)) {

		m_Flags.ClearFlag(c_MaximumInstanceCountExceeded);

		glNamedBufferData(GetSpriteIndexVBO(), GetInstanceCount() * sizeof(unsigned short), _spriteIndices, GL_DYNAMIC_DRAW);
		glNamedBufferData(GetRotationsVBO(), GetInstanceCount() * sizeof(float), _objectRotationsRad, GL_DYNAMIC_DRAW);
		glNamedBufferData(GetPositionsVBO(), GetInstanceCount() * 2 * sizeof(float), _pairsOfxyPositions, GL_DYNAMIC_DRAW);

		return;
	}

	
	UpdateRotationsBuffer(_objectRotationsRad, _arrayElementCount);
	UpdatePositionsBuffer(_pairsOfxyPositions, _arrayElementCount);
}


bool SoftBatch::UpdateSpriteIndexVBO(
	const unsigned short* _spriteIndices,
	const int _sheetIndex,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());
	
	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _spriteIndices == nullptr || _arrayElementCount >= GetInstanceCount()) {
		return false;
	}


	DEBUG_ASSERT(_sheetIndex < 128, "Indexing beyond maximum allowed sprite sheets per batch.");
	if (_sheetIndex >= m_SpriteSheets.size()) {
		DEBUG_ASSERT(0, "No sheet with index [%d] in batch with name [%s]. Highest index is [%s]!", _sheetIndex, dm_BatchName.c_str(), static_cast<int>(m_SpriteSheets.size()));
		return false;
	}

	std::vector<unsigned short> TemporaryBuffer;
	TemporaryBuffer.resize(_arrayElementCount);

	for (size_t i = 0; i < _arrayElementCount; i++) {
		unsigned short Index = 0;

		DEBUG_ASSERT(_spriteIndices[i] < 512, "Indexing beyond maximum allowed sprites per sheet.");

		Index |= _sheetIndex << 9;
		Index |= _spriteIndices[i];

		TemporaryBuffer[i] = Index;
	}

	glNamedBufferSubData(GetSpriteIndexVBO(), 0, GetInstanceCount() * sizeof(unsigned short), TemporaryBuffer.data());
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
	glBindVertexBuffer(2, GetSpriteIndexVBO(), 0, sizeof(unsigned short));
	glBindVertexBuffer(3, GetRotationsVBO(), 0, sizeof(float));
	glBindVertexBuffer(4, GetPositionsVBO(), 0, sizeof(float) * 2);
}


void SoftBatch::BindCommonVAO(){
	glBindVertexArray(s_VAO);
}


void SoftBatch::UnbindCommonVAO(){
	glBindVertexArray(0);
}
