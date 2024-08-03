#pragma once

#include <fstream>
#include <tuple>
#include <sstream>
#include <Physis.h>
#include "Extractor.h"

class FileExtractor : public Extractor
{
private:
	std::ifstream m_file;

public:
	FileExtractor(const char* filepath);
	~FileExtractor();

	std::tuple<InitialConditions, SystemConfig> Extract();
};

