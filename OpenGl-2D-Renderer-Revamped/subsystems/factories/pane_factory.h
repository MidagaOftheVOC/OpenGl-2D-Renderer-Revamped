#pragma once
#include "factory_interface.h"
#include "../../components/ui/primitives/types/pane.h"

class PaneFactory : public FactoryInterface{
public:

	PaneFactory() : FactoryInterface() {}

	PaneFactory(
		const ResourceService* res
	)
		: FactoryInterface(res)
	{}

	Pane CreatePane(
		float _width,
		float _height,
		float _cornerLength,
		const char* _skinName
	) const;

};

