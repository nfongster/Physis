#pragma once

#include "Physis.h"
#include <fstream>
#include <sstream>
#include <vector>

class BenchmarkEngine : public EngineBase
{
private:
	const std::string m_outdir;
	const std::chrono::duration<double, std::milli> m_render_time;
	std::vector<std::chrono::duration<double, std::milli>> m_durations;

protected:
	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

public:
	BenchmarkEngine(const SystemConfig& sc, const std::string& outdir, 
		const std::chrono::duration<double, std::milli>& render_time);

	~BenchmarkEngine();

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};