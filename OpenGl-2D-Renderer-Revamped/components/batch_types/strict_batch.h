#pragma once

#include "batch.h"



/*
*	StrictBatch:	A Batch for instanced rendering where all objects rendered have
*					strong resrtictions.
*	
*	Requirements for drawn objects:
*		-	placed in a grid, relative to each other
*		-	they're not rotated
*		-	have uniform scale
*		-	only need world space position to draw
*/

class StrictBatch : public Batch {

};

