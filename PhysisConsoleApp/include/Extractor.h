#pragma once
#include <tuple>
#include "InitialConditions.h"
#include "SystemConfig.h"

class Extractor
{
public:
	virtual ~Extractor() { }

	virtual std::tuple<InitialConditions, SystemConfig> Extract() = 0;
};

