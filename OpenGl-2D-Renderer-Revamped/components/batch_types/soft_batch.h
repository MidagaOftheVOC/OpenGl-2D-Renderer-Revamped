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
*		- optional, if type is set to FloatingQuad, the quad's vertices will be 
*		  altered to match the sampled UV region from the texture
*/


enum class SoftBatchType{
	FixedQuad,
	FloatingQuad
};

class SoftBatch : public BaseBatch{

	unsigned int m_SpriteIndexVBO = 0;
	unsigned int m_RotationsVBO = 0;
	unsigned int m_PositionsVBO = 0;


	const SoftBatchType m_Type;

public:

	SoftBatch(
		int _instanceCount,
		SoftBatchType _type = SoftBatchType::FixedQuad
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

		This means, if call UpdateBuffers(arr1, 0, arr3, _count) and _count exceeds the maximum,
		all buffers will be resized, but at the resizing process, only VBOs for
		arr1 and arr3 will have data, VBO of arr2 will remain with memory, initialised with a
		default value(usually 0s), until it's rebuffered with the proper
		Update...VBO() function.
	*/
	void UpdateBuffers(
		const unsigned short* _spriteIndices,
		const float* _objectRotationsRad,
		const float* _pairsOfxyPositions,
		const size_t _arrayElementCount = 0
	);

	
	//	Updates the UVRegions' VBO.
	//	Returns:	[false], if _spriteIndices is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount > m_BufferedInstanceCount
	//				[true], otherwise
	bool UpdateSpriteIndexVBO(
		const unsigned short* _spriteIndices,
		const unsigned short* _sheetIndices,
		const size_t _arrayElementCount = 0
	);


	//	Updates the Rotations' VBO.
	//	Returns:	[false], if _objectRotationsRad is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount > m_BufferedInstanceCount
	//				[true], otherwise
	bool UpdateRotationsBuffer(
		const float* _objectRotationsRad,
		const size_t _arrayElementCount = 0
	);


	//	Updates the Positions' VBO.
	//	Returns:	[false], if _pairsOfxyPositions is null OR c_MaximumInstanceCountExceeded is raised OR _arrayElementCount > m_BufferedInstanceCount
	//				[true], otherwise
	bool UpdatePositionsBuffer(
		const float* _pairsOfxyPositions,
		const size_t _arrayElementCount = 0
	);


	void BindUniqueBuffers() const;

private:

	static unsigned int s_VAO;

public:

	static void InitialiseCommonVAO();


	static void BindCommonVAO();


	static void UnbindCommonVAO();

public:

	unsigned int GetSpriteIndexVBO() const { return m_SpriteIndexVBO; }
	unsigned int GetRotationsVBO() const { return m_RotationsVBO; }
	unsigned int GetPositionsVBO() const { return m_PositionsVBO; }
	SoftBatchType GetType() const { return m_Type; }


	virtual ~SoftBatch() {}

};

