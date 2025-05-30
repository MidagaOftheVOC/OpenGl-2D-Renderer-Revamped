#include "base_batch.h"


unsigned int BaseBatch::c_NotInitialised =					1 << 0;



unsigned int BaseBatch::c_MaximumInstanceCountExceeded =	1 << 16;




BaseBatch::BaseBatch(
	const SpriteSheet* _spriteSheet,
	int _instanceCount
) :
	m_SpriteSheet(_spriteSheet)
{
	SetInstanceCount(_instanceCount);
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