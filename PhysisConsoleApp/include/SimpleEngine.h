#pragma once

#include "Physis.h"
#include <fstream>
#include <sstream>

class SimpleEngine : public EngineBase
{
private:
	const std::string m_outdir;
	const std::chrono::duration<double, std::milli> m_render_time;
	std::vector<std::chrono::duration<double, std::milli>> m_durations;

public:
	SimpleEngine(const SystemConfig& sc, const std::string& outdir, 
		const std::chrono::duration<double, std::milli>& render_time);

	~SimpleEngine();

	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};