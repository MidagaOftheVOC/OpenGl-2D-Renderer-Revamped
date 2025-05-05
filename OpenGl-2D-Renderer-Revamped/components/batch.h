#pragma once


#include "drawable.h"

template<int _elementCount>
class Batch {

	//	Copying and storign this data is more expensive than indexing
	//	from the Renderer obj BUT it's also more efficient since
	//	it only contains UVRegion objects rather then SpriteRegions
	UVRegion m_RegionArray[_elementCount];

	SpriteSheet* m_SpriteSheet = nullptr;

	Shader* m_Shader = nullptr;

public:

	Batch() {}


	void LoadDrawableArray(
		const std::vector<Drawable>& _drawableArray
	);

	void LoadDrawableArray(
		const Drawable* _drawableArray,
		const int _nCount
	);

private:

	void LoadData(
		const Drawable* _drawableArray,
		const int _nCount
	);

};

