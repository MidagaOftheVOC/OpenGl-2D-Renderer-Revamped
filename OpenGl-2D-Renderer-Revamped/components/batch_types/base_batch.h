#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"



//	Common batch methods and data

class BaseBatch {
protected:
	
	std::vector<const SpriteSheet*> m_SpriteSheets;
	
	int m_InstanceCount = 0;
	int m_BufferedInstanceCount = 0;

	FlagTracker m_Flags = FlagTracker(c_NotInitialised);

protected:	//	Flags

	//	Errors
	static unsigned int c_NotInitialised;


	//	Exceptions
	static unsigned int c_MaximumInstanceCountExceeded;

protected:	//	UBOs

	unsigned int m_SheetUVRegionsUBO = 0;
	unsigned int m_SheetIndexOffsetsUBO = 0;

public:

	BaseBatch(
		int _instanceCount
	);


	void BindUBOs() const;


	void BufferUBOs();


	virtual void ActivateTextures(
		const char* _sampler2DArrayUniformName
	) const;


	//	Add a SpriteSheet* to this batch. It will be used in every
	//	draw call made with this batch.
	void AddSheetToBatch(
		const SpriteSheet* _spriteSheet
	);


	virtual void InitialiseBuffers() = 0;


	virtual void DeleteBuffers() = 0;


	//	Can raise the c_MaximumInstanceCountExceeded flag if _newInstanceCount 
	//	exceeds the maximum instance count, which indicates buffer resizing is mandatory
	//
	//	If 0 is passed, the instance count isn't changed.
	bool SetInstanceCount(
		int _newInstanceCount
	);


	//	This function is used to pack sprite and sheet index data into (unsigned short) types.
	//	Arrays _spriteIndices and _sheetIndices with element counts equal to _arrayElementCount
	//	are passed. Then, the OUT_finishedArray, which is an array with size at least 
	//	_arrayElementCount, is filled with the finished data.
	//
	//	Current non-flexible format:
	//	Bits 0-8 : Sprite index
	//	Bits 9-15: Sheet index
	//
	//	TODO: Make this more flexible, maximum sampler2D in one drawcall is system dependant.
	//	Currently, the sampler2D array in the shaders has a maximum size of 32.
	//
	//	Returns:	[false], if either array pointer is null OR _arrayElementCount > m_BufferedInstanceCount
	//				[true], otherwise
	bool PackIndicesTogether(
		const unsigned short* _spriteIndices,
		const unsigned short* _sheetIndices,
		const size_t _arrayElementCount,
		unsigned short* OUT_finishedArray
	) const;


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

public:

	//const std::vector<const SpriteSheet*> GetSpriteSheets() const { return m_SpriteSheets; }
	const int GetInstanceCount() const { return m_InstanceCount; }
	
	//	Returns the maximum number of per-instance data pieces the current buffers can 
	//	hold without overflowing, indicating it's safe to rebuffer the VBOs with bytes
	//	from their beginning, up to this value, times the size of their respective
	//	elements in bytes, without overflowing into uninitialised memory regions.
	const size_t GetBufferedInstanceCount() const { return static_cast<size_t>(m_BufferedInstanceCount); }

	const SpriteSheet* GetSpecialSheetPointer() const;


	const size_t GetSheetCount() const { return m_SpriteSheets.size(); }


	virtual ~BaseBatch() {}

};



