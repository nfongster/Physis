#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include "EngineBase.h"

class PHYSIS_API BenchmarkEngine : public EngineBase
{
private:
	const std::string m_outdir;
	const std::chrono::duration<double, std::milli> m_render_time;
	std::vector<std::chrono::duration<double, std::milli>> m_durations;
	std::map<unsigned int, std::vector<InitialConditions>> m_history;

protected:
	void OnCompletion();

	void Render();
	void Interpolate(const double& factor);

public:
	BenchmarkEngine(const TimeConfig& sc, const std::string& outdir, 
		const std::chrono::duration<double, std::milli>& render_time);

	~BenchmarkEngine();

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};