#include "base_batch.h"


unsigned int BaseBatch::c_NotInitialised =					1 << 0;



unsigned int BaseBatch::c_MaximumInstanceCountExceeded =	1 << 16;



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
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_SpriteSheets[i]->GetTextureBufferID());
		TexUnits.push_back(static_cast<int>(i));
	}
	CheckGLErrors();
	Shader->SetIntArray(_sampler2DArrayUniformName, TexUnits.data(), static_cast<const int>(TexUnits.size()));
	CheckGLErrors();
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

	if (!GLdiagnostics::IsBuffer(m_SheetIndexOffsetsUBO)) {
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

