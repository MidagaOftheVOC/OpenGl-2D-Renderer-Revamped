#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include "base_batch.h"

/*
*	SoftBatch:	A batch for rendering many objects with looser 
*				restrictions than the StrictBatch
* 
*	Supports per-instance:
*		- sprite UVs
*		- rotation
*		- position
*/

class SoftBatch : public BaseBatch{

	unsigned int m_UVRegionsVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;

public:

	SoftBatch(
		const SpriteSheet* _spriteSheet,
		int _instanceCount
	);


	virtual void InitialiseBuffers();


	virtual void DeleteBuffers();

	/*
		Either array element can be set to /nullptr/, in that case, it is not updated.
		Same goes for _arrayElementCount, it it's 0, we assume no changes to the array
		lengths have occured and continue as usual.

		_arrayElementCount represents the element counts of all passed arrays,
		if they're not equal, we get leaks or uninitialised memory.
		
		The function calls SoftBatch::SetInstanceCount(_arrayElementCount).
		If the max instance count is exceeded, we resize the buffers
		! AND ONLY THE NON-NULLPTR ARRAYS WILL BE BUFFERED TO THE GPU !

		This means, if call UpdateBuffers(arr1, null, arr3, _count) and _count exceeds the maximum,
		all buffers will be resized, but at the resizing process, only VBOs for
		arr1 and arr3 will have data, VBO of arr2 will remain uninitialised and you'll have to
		seperately call the associated Update...Buffer().
	*/
	void UpdateBuffers(
		const int* _spriteIndices,
		const float* _objectRotationsRad,
		const float* _pairsOfxyPositions,
		const size_t _arrayElementCount = 0
	);

	
	//	Updates the UVRegions' VBO.
	//	Returns:	[false], if _spriteIndices is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount >= m_InstanceCount
	//				[true], otherwise
	bool UpdateUVRegionVBO(
		const int* _spriteIndices,
		const size_t _arrayElementCount = 0
	);


	//	Updates the Rotations' VBO.
	//	Returns:	[false], if _objectRotationsRad is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount >= m_InstanceCount
	//				[true], otherwise
	bool UpdateRotationsBuffer(
		const float* _objectRotationsRad,
		const size_t _arrayElementCount = 0
	);


	//	Updates the Positions' VBO.
	//	Returns:	[false], if _pairsOfxyPositions is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount >= m_InstanceCount
	//				[true], otherwise
	bool UpdatePositionsBuffer(
		const float* _pairsOfxyPositions,
		const size_t _arrayElementCount = 0
	);


	void BindUniqueBuffers() const;

#ifdef DEBUG__CODE

	std::string dm_BatchName;
	void DEBUG_SetName(
		const char* _newName
	) {
		dm_BatchName = _newName;
	}

#else

	inline void DEBUG_SetName(
		const char* DEBUG_MODE_IS_DISABLED_newName
	) {}

#endif

private:

	static unsigned int s_VAO;

public:

	static void InitialiseCommonVAO();


	static void BindCommonVAO();


	static void UnbindCommonVAO();

public:

	unsigned int GetUVRegionsVBO() const { return m_UVRegionsVBO; }
	unsigned int GetRotationsVBO() const { return m_RotationsVBO; }
	unsigned int GetPositionsVBO() const { return m_PositionsVBO; }

	virtual ~SoftBatch() {}

};

