#include "../main_renderer.h"



/*
* 
*	Test snippets:	This is a place to dump easy setups that return initialised pieces of data,
*					prepared to be rendered for testing purposes.
* 
* 
*/


static StrictBatch GetInitialisedStrictBatch(
	const SpriteSheet* _sheet
) {
	StrictBatch self(4, 2);
	self.AddSheetToBatch(
		_sheet
	);

	self.InitialiseBuffers();

	int arr[4] = { 0 };
	self.UpdateBuffer(arr, 4);

	return self;
}


static SoftBatch GetInitialisedSoftBatch(
	const SpriteSheet* _sheet1,
	const SpriteSheet* _sheet2
) {
	const unsigned short indices[] = { 0, 0, 0, 0 };
	float rotations[] = { 0.f, 1.f, 2.f, 3.f };
	float PositionPais[] = { 20.f, 20.f, 120.f, 120.f, 220.f, 220.f, 320.f, 320.f };

	SoftBatch fq = SoftBatch(4, SoftBatchType::FloatingQuad);
	fq.InitialiseBuffers();
	fq.AddSheetToBatch(
		_sheet1
	);

	fq.AddSheetToBatch(
		_sheet2
	);
	fq.BufferUBOs();

	fq.UpdateBuffers(
		nullptr,
		rotations,
		PositionPais,
		4
	);

	unsigned short sprites[] = { 0, 0, 0, 0 };
	unsigned short sheets[] = { 0, 0, 1, 1 };

	fq.UpdateSpriteIndexVBO(sprites, sheets, 4);

	return fq;
}


