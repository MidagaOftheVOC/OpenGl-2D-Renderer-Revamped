#pragma once


#include "../../common/common.h"
#include "../../common/standard_quad.h"


#include "base_batch.h"
#include "batch_instance_primitives.h"

/*
This type will be used for batch-rendering of 9-piece rectangles, primarely
associated with GUI elements such as a window's background, buttons and input fields.


Layout:
	@0	4 vertices, each with 0s and/or 1s only, used for interpreting other fields

	@1	1* unsigned int /    instance - sprite information(sprite + texture indices for each quad within)
	@2	2* float		/    instance - offset from the batch's drawcall's position
	@3	2* float		/    instance - xy dimensions of the quad
	@4	1* float		/ 9* instance - common z-coord for 9 instances

	Note: Z buffer is relative to the furthest Z coord from the screen.
*/
class UIBatch : public BaseBatch {

	unsigned int m_SpriteInformationVBO = 0;
	unsigned int m_PositionsVBO = 0;
	unsigned int m_DimensionsVBO = 0;
	unsigned int m_LayersVBO = 0;

public:

	UIBatch(
		int _instanceCount
	);


	virtual void InitialiseBuffers();


	virtual void DeleteBuffers();


	void UpdateBuffers(
		const SpriteInformation* _spriteInformation,
		const float* _pairsOfXYpositions,
		const float* _pairsOfXYdimensions,
		const float* _zLayers,
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

	virtual ~UIBatch() {}

};



