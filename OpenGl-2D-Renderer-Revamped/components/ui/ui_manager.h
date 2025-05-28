#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "../batch_types/soft_batch.h"


class UImanager {

	//	From the renderer-assigned usable Z space
	//	This here tells us where we can and cannot draw UI elements

	float m_LowerBoundOfZlayerDistribution = 0.f;
	float m_UpperBoundOfZlayerDistribution = 0.f;

	//	Layers of panes will be stored as ints, final Z value
	//	taken by LayerIndex * (this float value)
	float m_CalculatedZlayerStep = 0.f;


private:

	//	TODO:
	//	To enable this GUI to function fully, we'll probably need
	//	to enable soft batches to work with multiple textures.
	//	
	//	OR
	// 
	//	Add a long put-off functionality to Sprite sheets:
	//	ability to load images paired with a config file
	//	which tells us precisely what and where we sample
	//	within it. Now is probably the time to do it.
	//	



public:
	
	void MutateSoftBatchForUI(
		SoftBatch& OUT_batch
	);

};
