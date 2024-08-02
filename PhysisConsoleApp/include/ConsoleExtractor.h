#pragma once
#include <tuple>
#include "InitialConditions.h"
#include "SystemConfig.h"

class ConsoleExtractor
{
public:
	ConsoleExtractor();
	~ConsoleExtractor();

	std::tuple<InitialConditions, SystemConfig> Extract();
};

