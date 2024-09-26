#pragma once

#include <iostream>
#include <tuple>
#include <Physis.h>
#include "Extractor.h"
#include "Utility.h"

class ConsoleExtractor : public Extractor
{
public:
	ConsoleExtractor();
	~ConsoleExtractor();

	std::tuple<KinematicParameters, TimeConfig, int> Extract();
};

