#pragma once
#include <fstream>
#include <tuple>
#include "InitialConditions.h"
#include "SystemConfig.h"

class FileExtractor
{
private:
	std::ifstream m_file;

public:
	FileExtractor(const char* filepath);
	~FileExtractor();

	std::tuple<InitialConditions, SystemConfig> Extract();
};

