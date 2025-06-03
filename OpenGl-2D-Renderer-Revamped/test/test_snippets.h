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




