#pragma once
#include <tuple>
#include "Extractor.h"
#include "InitialConditions.h"
#include "SystemConfig.h"

class ConsoleExtractor : public Extractor
{
public:
	ConsoleExtractor();
	~ConsoleExtractor();

	std::tuple<InitialConditions, SystemConfig> Extract();
};

