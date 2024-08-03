#pragma once
#include <tuple>
#include <Physis.h>

class Extractor
{
public:
	virtual ~Extractor() { }

	virtual std::tuple<InitialConditions, SystemConfig> Extract() = 0;
};

