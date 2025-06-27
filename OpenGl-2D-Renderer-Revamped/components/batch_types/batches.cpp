#include "base_batch.h"
#include "strict_batch.h"
#include "soft_batch.h"
#include "free_batch.h"
#include "ui_batch.h"




/*		DATA TYPES		*/


uint16_t SpriteInformation::s_SheetSpriteBitmask = 0;

uint16_t SpriteInformation::s_SheetIndexBits = 5;
uint16_t SpriteInformation::s_SpriteIndexBits = 11;


void SpriteInformation::InitialiseMasks() {
	for (int i = 0; i < s_SheetIndexBits; i++) {
		s_SheetSpriteBitmask |= 1ui16 << (15 - i);
	}
}


SpriteInformation::SpriteInformation(
	unsigned int _sheetIndex,
	unsigned int _spriteIndex
) {

	uint16_t self = 0;

	self |= _sheetIndex << (16 - s_SheetIndexBits);
	self |= _spriteIndex;

	m_SheetSpriteIndexData = self;
}


uint16_t SpriteInformation::GetSheetIndex() const {
	return uint16_t(m_SheetSpriteIndexData & s_SheetSpriteBitmask) >> (16 - s_SheetIndexBits);
}


uint16_t SpriteInformation::GetSpriteIndex() const {
	return uint16_t(m_SheetSpriteIndexData & ~s_SheetSpriteBitmask);
}



/*		BASE BATCH		*/

unsigned int BaseBatch::c_NotInitialised = 1 << 0;


unsigned int BaseBatch::c_MaximumInstanceCountExceeded = 1 << 16;


BaseBatch::BaseBatch(
	int _instanceCount
) {
	SetInstanceCount(_instanceCount);
}


void BaseBatch::ActivateTextures(
	const char* _sampler2DArrayUniformName
) const {
	if (m_SpriteSheets.empty()) return;

	const Shader* Shader = m_SpriteSheets[0]->GetShader();
	std::vector<int> TexUnits;

	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + static_cast<unsigned int>(i));
		glBindTexture(GL_TEXTURE_2D, m_SpriteSheets[i]->GetTextureBufferID());
		TexUnits.push_back(static_cast<int>(i));
	}

#ifdef DEBUG__CODE
	CheckGLErrors();
#endif

	Shader->SetIntArray(_sampler2DArrayUniformName, TexUnits.data(), static_cast<const int>(TexUnits.size()));

#ifdef DEBUG__CODE
	CheckGLErrors();
#endif
}


void BaseBatch::AddSheetToBatch(
	const SpriteSheet* _spriteSheet
) {
	DEBUG_ASSERT(_spriteSheet != nullptr, "Null pointers passed to batch method for adding sheets.");

	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		if (_spriteSheet->GetShader() != m_SpriteSheets[i]->GetShader()) {
			DEBUG_ASSERT(0, "Batch w/ name [%s] has tried to append SpriteSheet objects with different shaders!", dm_BatchName.c_str());
			return;
		}
	}

	m_SpriteSheets.emplace_back(
		_spriteSheet
	);
}


bool BaseBatch::SetInstanceCount(
	int _newInstanceCount
) {
	DEBUG_ASSERT(_newInstanceCount > 0 && _newInstanceCount < 0x7fffffff, "SoftBatch setting invalid value for Instance count.");
	//DEBUG_WARN(_newInstanceCount == GetInstanceCount(), "Setting Instance count to the same number in SoftBatch with name [%s].", dm_BatchName.c_str());


	if (_newInstanceCount == m_InstanceCount) return true;

	if (_newInstanceCount <= 0 || _newInstanceCount > 0x7fffffff) {
		return false;
	}

	if (_newInstanceCount > m_BufferedInstanceCount) {
		m_Flags.SetFlag(c_MaximumInstanceCountExceeded);
		m_BufferedInstanceCount = _newInstanceCount;
	}

	m_InstanceCount = _newInstanceCount;
	return true;
}


const SpriteSheet* BaseBatch::GetSpecialSheetPointer() const {
	DEBUG_ASSERT(m_SpriteSheets.size() > 0, "Batch has empty Sheet container.");

	return m_SpriteSheets[0];
}


//	TODO:	Marked for removal
bool BaseBatch::PackIndicesTogether(
	const unsigned short* _spriteIndices,
	const unsigned short* _sheetIndices,
	const size_t _arrayElementCount,
	unsigned short* OUT_finishedArray
) const {

	if (!_spriteIndices || !_sheetIndices || !OUT_finishedArray || _arrayElementCount > GetBufferedInstanceCount()) return false;

	for (size_t i = 0; i < _arrayElementCount; i++) {

		unsigned short Self = 0;

		if (_sheetIndices[i])

			Self |= _sheetIndices[i] << 9;
		Self |= _spriteIndices[i];

		OUT_finishedArray[i] = Self;
	}
	return true;
}


void BaseBatch::BufferUBOs() {
	if (m_SpriteSheets.empty()) return;

	CheckGLErrors();

	if (!GLdiagnostics::IsBuffer(m_SheetIndexOffsetsUBO)) {
		DEBUG_ASSERT(0, "Buffering UBOs that aren't initialised.");
		return;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_SheetUVRegionsUBO);

	std::vector<UVRegion> UVs;
	for (const auto& sheet : m_SpriteSheets) {
		const auto& arr = sheet->GetUVRegionArray();
		UVs.insert(UVs.end(), arr.begin(), arr.end());
	}

	glBufferData(GL_UNIFORM_BUFFER, UVs.size() * sizeof(UVRegion), UVs.data(), GL_STATIC_DRAW);


	//	for the (std140) buffer alignment requiring 16 byte blocks within the shader code
	//	Note: UVRegion doesn't need it since it's already 4 * float = 16 bytes
	struct alignas(16) PaddedInteger {
		int value;
		PaddedInteger(int _val) : value(_val) {}
	};

	glBindBuffer(GL_UNIFORM_BUFFER, m_SheetIndexOffsetsUBO);
	//	TODO: check if it's necessary to record the last sheet's offset
	std::vector<PaddedInteger> Offsets;
	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		Offsets.emplace_back(static_cast<int>(m_SpriteSheets[i]->GetContainedSpriteCount()));
	}

	glBufferData(GL_UNIFORM_BUFFER, Offsets.size() * sizeof(PaddedInteger), Offsets.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	const Shader* ShaderObject = GetSpecialSheetPointer()->GetShader();



	unsigned int Program = ShaderObject->GetShaderId();

	unsigned int UBO_UVRegions = ShaderObject->GetUniformBlockLocation("ubo_UVRegions");
	unsigned int UBO_SheetOffsets = ShaderObject->GetUniformBlockLocation("ubo_SheetOffsets");

	CheckGLErrors();
	glUniformBlockBinding(Program, UBO_UVRegions, 0);
	glUniformBlockBinding(Program, UBO_SheetOffsets, 1);
	CheckGLErrors();
}


void BaseBatch::BindUBOs() const {
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_SheetUVRegionsUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_SheetIndexOffsetsUBO);
}


SpriteInformation BaseBatch::DeriveSprite(
	const char* _sheetName,
	const char* _spriteNameWithinSheet
) const {

	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		if (FastStringCompare(m_SpriteSheets[i]->GetName().c_str(), _sheetName)) {
			unsigned short Result = m_SpriteSheets[i]->GetSpriteIndexByName(_spriteNameWithinSheet);
			if (Result != gc_ui16ErrorCode) {
				return SpriteInformation(i, Result);
			}
			return SpriteInformation(0, 0);
		}
	}

	return SpriteInformation(0, 0);
}



/*		STRICT BATCH		*/


unsigned int StrictBatch::s_VAO = 0;


StrictBatch::StrictBatch(
	int _instanceCount,
	int _spriteCountPerRow
) :
	BaseBatch(_instanceCount),
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
	GetSpecialSheetPointer()->TransformIndicesToUVRegionArray(_spriteIndexArray, GetInstanceCount(), UVRegionArray);

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


/*		SOFT BATCH			*/


unsigned int SoftBatch::s_VAO = 0;


SoftBatch::SoftBatch(
	int _instanceCount,
	SoftBatchType _type
) :
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
	glDeleteBuffers(1, &m_SheetIndexOffsetsUBO);
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

	//	Update all, even with null data.
	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded)) {

		m_Flags.ClearFlag(c_NotInitialised);
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
	const unsigned short* _sheetIndices,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());

	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _spriteIndices == nullptr || _arrayElementCount > GetBufferedInstanceCount()) {
		return false;
	}

	std::vector<unsigned short> TemporaryBuffer;
	TemporaryBuffer.resize(_arrayElementCount);

	PackIndicesTogether(
		_spriteIndices,
		_sheetIndices,
		_arrayElementCount,
		TemporaryBuffer.data()
	);

	glNamedBufferSubData(GetSpriteIndexVBO(), 0, GetInstanceCount() * sizeof(unsigned short), TemporaryBuffer.data());
	return true;
}


bool SoftBatch::UpdateRotationsBuffer(
	const float* _objectRotationsRad,
	const size_t _arrayElementCount
) {
	DEBUG_ASSERT(!m_Flags.CheckFlag(c_NotInitialised), "Attempting update of non-initialised buffer for SoftBatch with name [%s].", dm_BatchName.c_str());

	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _objectRotationsRad == nullptr || _arrayElementCount > GetBufferedInstanceCount()) {
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

	if (m_Flags.CheckFlag(c_MaximumInstanceCountExceeded) || _pairsOfxyPositions == nullptr || _arrayElementCount > GetBufferedInstanceCount()) {
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


void SoftBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void SoftBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}



/*		FREE BATCH			*/


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
			m_SpriteInformationVBO, sizeof(SpriteInformation) * GetInstanceCount(), _spriteInformation, GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_RotationsVBO, sizeof(float) * GetInstanceCount(), _rotationsArray, GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_PositionsVBO, sizeof(float) * 2 * GetInstanceCount(), _pairsOfXYpositions, GL_DYNAMIC_DRAW);

		glNamedBufferData(
			m_VertexDimensionsVBO, sizeof(float) * 2 * GetInstanceCount(), _pairsOfXYdimensions, GL_DYNAMIC_DRAW);

		return;
	}

	if (_spriteInformation)
		glNamedBufferSubData(m_SpriteInformationVBO, 0, sizeof(SpriteInformation) * GetInstanceCount(), _spriteInformation);

	if (_rotationsArray)
		glNamedBufferSubData(m_RotationsVBO, 0, sizeof(float) * GetInstanceCount(), _rotationsArray);

	if (_pairsOfXYpositions)
		glNamedBufferSubData(m_PositionsVBO, 0, sizeof(float) * 2 * GetInstanceCount(), _pairsOfXYpositions);

	if (_pairsOfXYdimensions)
		glNamedBufferSubData(m_VertexDimensionsVBO, 0, sizeof(float) * 2 * GetInstanceCount(), _pairsOfXYdimensions);
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


/*		UI	BATCH		*/

unsigned int UIBatch::s_VAO = 0;


UIBatch::UIBatch(
	int _instanceCount
) :
	BaseBatch(_instanceCount)
{}


void UIBatch::InitialiseBuffers() {
	if (GLdiagnostics::IsBuffer(m_SpriteInformationVBO)) {
		DEBUG_ASSERT(0, "Batch [%s] is initialised again.", dm_BatchName.c_str());
		return;
	}
		
	glCreateBuffers(1, &m_SpriteInformationVBO);
	glCreateBuffers(1, &m_PositionsVBO);
	glCreateBuffers(1, &m_DimensionsVBO);
	glCreateBuffers(1, &m_LayersVBO);

	glCreateBuffers(1, &m_SheetIndexOffsetsUBO);
	glCreateBuffers(1, &m_SheetUVRegionsUBO);
}


void UIBatch::DeleteBuffers() {
	glDeleteBuffers(1, &m_SpriteInformationVBO);
	glDeleteBuffers(1, &m_PositionsVBO);
	glDeleteBuffers(1, &m_DimensionsVBO);
	glDeleteBuffers(1, &m_LayersVBO);

	glDeleteBuffers(1, &m_SheetIndexOffsetsUBO);
	glDeleteBuffers(1, &m_SheetUVRegionsUBO);

	m_SpriteInformationVBO = 0;
	m_PositionsVBO = 0;
	m_DimensionsVBO = 0;
	m_LayersVBO = 0;

	m_SheetIndexOffsetsUBO = 0;
	m_SheetUVRegionsUBO = 0;
}


void UIBatch::InitialiseCommonVAO() {

	if (GLdiagnostics::IsVertexArray(s_VAO)) {
		DEBUG_ASSERT(0, "UIBatch tries to reinitialise common VAO.");
		return;
	}

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	//	@0
	//	TODO: check this
	//g_StandardQuad.BindUnmodifiedStandardQuadBufferAt(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_StandardQuad.m_InterpolationBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	//	@element
	g_StandardQuad.BindElementIndexArray();

	//	@1
	glBindVertexBuffer(1, 0, 0, 1 * sizeof(SpriteInformation));
	glVertexAttribIFormat(1, 1, GL_UNSIGNED_SHORT, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribBinding(1, 1);
	glVertexBindingDivisor(1, 1);


	//	@2
	glBindVertexBuffer(2, 0, 0, 2 * sizeof(float));
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribBinding(2, 2);
	glVertexBindingDivisor(2, 1);

	// @3
	glBindVertexBuffer(3, 0, 0, 2 * sizeof(float));
	glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribBinding(3, 3);
	glVertexBindingDivisor(3, 1);

	// @4
	glBindVertexBuffer(4, 0, 0, 1 * sizeof(float));
	glVertexAttribFormat(4, 1, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribBinding(4, 4);
	glVertexBindingDivisor(4, 9);


	glBindVertexArray(0);
}


void UIBatch::UpdateBuffers(
	const SpriteInformation* _spriteInformation,
	const float* _pairsOfXYpositions,
	const float* _pairsOfXYdimensions,
	const float* _zLayers,
	const size_t _arrayElementCount
) {
	SetInstanceCount(static_cast<int>(_arrayElementCount));

	DEBUG_ASSERT(_arrayElementCount % 9 == 0, "UIBatch has instance count not divisibly by 9.");

	int zLayerCount = GetInstanceCount() / 9;
	int zLayerSize = zLayerCount * sizeof(float);
	
	if (m_Flags.CheckAndClearFlag(c_MaximumInstanceCountExceeded)) {
		m_Flags.ClearFlag(c_NotInitialised);

		glNamedBufferData(m_SpriteInformationVBO,
			GetInstanceCount() * sizeof(SpriteInformation), _spriteInformation, GL_DYNAMIC_DRAW);
		
		glNamedBufferData(m_PositionsVBO,
			GetInstanceCount() * sizeof(float) * 2, _pairsOfXYpositions, GL_DYNAMIC_DRAW);
		
		glNamedBufferData(m_DimensionsVBO,
			GetInstanceCount() * sizeof(float) * 2, _pairsOfXYdimensions, GL_DYNAMIC_DRAW);
		
		glNamedBufferData(m_LayersVBO, 
			zLayerSize, _zLayers, GL_DYNAMIC_DRAW);

		return;
	}

	if(_spriteInformation)
		glNamedBufferSubData(m_SpriteInformationVBO, 0, GetInstanceCount() * sizeof(SpriteInformation), _spriteInformation);
	
	if (_pairsOfXYpositions)
		glNamedBufferSubData(m_PositionsVBO, 0, GetInstanceCount() * sizeof(float) * 2, _pairsOfXYpositions);
	
	if (_pairsOfXYdimensions)
		glNamedBufferSubData(m_DimensionsVBO, 0, GetInstanceCount() * sizeof(float) * 2, _pairsOfXYdimensions);
	
	if (_zLayers)
		glNamedBufferSubData(m_LayersVBO, 0, zLayerSize, _zLayers);
}


void UIBatch::BindUniqueBuffers() const {
	glBindVertexBuffer(1, m_SpriteInformationVBO, 0, sizeof(SpriteInformation));
	glBindVertexBuffer(2, m_PositionsVBO, 0, sizeof(float) * 2);
	glBindVertexBuffer(3, m_DimensionsVBO, 0, sizeof(float) * 2);
	glBindVertexBuffer(4, m_LayersVBO, 0, sizeof(float));
}


void UIBatch::BindCommonVAO() {
	glBindVertexArray(s_VAO);
}


void UIBatch::UnbindCommonVAO() {
	glBindVertexArray(0);
}


