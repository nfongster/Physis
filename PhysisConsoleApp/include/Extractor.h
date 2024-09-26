#pragma once
#include <tuple>
#include <Physis.h>

class Extractor
{
public:
	virtual ~Extractor() { }

	virtual std::tuple<KinematicParameters, TimeConfig, int> Extract() = 0;
};

