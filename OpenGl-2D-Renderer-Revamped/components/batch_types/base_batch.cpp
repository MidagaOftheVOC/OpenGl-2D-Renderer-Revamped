#include "base_batch.h"


unsigned int BaseBatch::c_NotInitialised =					1 << 0;



unsigned int BaseBatch::c_MaximumInstanceCountExceeded =	1 << 16;




BaseBatch::BaseBatch(
	int _instanceCount
) {
	SetInstanceCount(_instanceCount);
}


void BaseBatch::ActivateTextures() const {
	if (m_SpriteSheets.empty()) return;

	const Shader* Shader = m_SpriteSheets[0]->GetShader();
	std::vector<int> TexUnits;

	
	for (size_t i = 0; i < m_SpriteSheets.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_SpriteSheets[i]->GetTextureBufferID());
		TexUnits.push_back(static_cast<int>(i));
	}
	CheckGLErrors();
	Shader->SetIntArray("u_Texture", TexUnits.data(), static_cast<const int>(TexUnits.size()));
	CheckGLErrors();
}


void BaseBatch::AddSheetToBatch(
	const SpriteSheet* _spriteSheet
) {

	DEBUG_ASSERT(_spriteSheet != nullptr, "Null pointers passed to batch method for adding sheets.");

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
	if (_newInstanceCount == 0) return true;

	if (_newInstanceCount < 0 || _newInstanceCount > 0x7fffffff) {
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
