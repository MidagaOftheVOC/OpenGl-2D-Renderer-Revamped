#include "batch.h"


template<int _elementCount>
void Batch<_elementCount>::LoadDrawableArray(
	const std::vector<Drawable>& _drawableArray
) {
	LoadData(
		_drawableArray.data(),
		_drawableArray.size()
	);
}


template<int _elementCount>
void Batch<_elementCount>::LoadDrawableArray(
	const Drawable* _drawableArray,
	const int _nCount
) {
	LoadData(
		_drawableArray,
		_nCount
	);
}


template<int _elementCount>
void Batch<_elementCount>::LoadData(
	const Drawable* _drawableArray,
	const int _nCount
) {
	DEBUG_ASSERT(_nCount <= _elementCount, "Batch obj with more elements than in template arg.\n");

	for (int i = 0; i < _nCount; i++) {
		//m_RegionArray[i] = *(_drawableArray + i).GetSpriteRegion().Region;
	}


}
