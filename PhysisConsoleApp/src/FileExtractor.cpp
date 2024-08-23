#include "FileExtractor.h"

FileExtractor::FileExtractor(const char* filepath) 
	: m_file(std::ifstream(filepath)) { }

FileExtractor::~FileExtractor()
{
	m_file.close();
}

std::tuple<InitialConditions, SystemConfig, int> FileExtractor::Extract()
{
	double r0x, r0y, v0x, v0y, a0x, a0y, t_total, dt, t_scale;
	std::string line;
	int index;
	int num_particles = 1;

	// Parse metadata
	std::getline(m_file, line);
	std::string token;
	std::stringstream ss = std::stringstream(line);
	while (std::getline(ss, token, ','))
	{
		if (token == "t")
		{
			std::getline(ss, token, ',');
			t_total = atof(token.c_str());
		}
		else if (token == "dt")
		{
			std::getline(ss, token, ',');
			dt = atof(token.c_str());
		}
		else if (token == "scale")
		{
			std::getline(ss, token, ',');
			t_scale = atof(token.c_str());
		}
		else if (token == "nump")
		{
			std::getline(ss, token, ',');
			num_particles = atof(token.c_str());
		}
	}

	std::getline(m_file, line);
	// TODO: parse header if we want to support different format types
	// TODO: support multiple particle init conditions w/o conflicting with 'nump' token above
	while (std::getline(m_file, line))
	{
		float* initial_conditions = new float[7];
		int i = 0;
		ss = std::stringstream(line);
		while (std::getline(ss, token, ','))
		{
			initial_conditions[i++] = atof(token.c_str());
		}
		index = (int)initial_conditions[0];
		r0x = initial_conditions[1];
		r0y = initial_conditions[2];
		v0x = initial_conditions[3];
		v0y = initial_conditions[4];
		a0x = initial_conditions[5];
		a0y = initial_conditions[6];
		delete[] initial_conditions;
	}

	return 
	{
		InitialConditions(Vec2(r0x, r0y), Vec2(v0x, v0y), Vec2(a0x, a0y)),
		SystemConfig(t_total, dt, t_scale),
		num_particles
	};
}
