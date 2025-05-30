#pragma once

#include "../../common/common.h"
#include "../../common/standard_quad.h"



//	Common batch methods and data


class BaseBatch {
protected:
	
	const SpriteSheet* m_SpriteSheet = nullptr;
	
	int m_InstanceCount = 0;
	int m_BufferedInstanceCount = 0;

	FlagTracker m_Flags = FlagTracker(c_NotInitialised);

protected:	//	Flags

	//	Errors
	static unsigned int c_NotInitialised;


	//	Exceptions
	static unsigned int c_MaximumInstanceCountExceeded;

public:

	BaseBatch(
		const SpriteSheet* _spriteSheet,
		int _instanceCount
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

	const SpriteSheet* GetSheet() const { return m_SpriteSheet; }
	const int GetInstanceCount() const { return m_InstanceCount; }

	virtual ~BaseBatch() {}

};



