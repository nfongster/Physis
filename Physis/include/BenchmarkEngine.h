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
	// key: particle ID.  value: list of (timestamp, kinematic params)
	// TODO: Create struct
	std::map<unsigned int, std::vector<std::tuple<double, KinematicParameters>>> m_history;

protected:
	void OnCompletion();
	void Render();
	void Interpolate(const double& factor);

public:
	BenchmarkEngine(const TimeConfig& config, 
					const std::string& outdir, 
					const std::chrono::duration<double>& render_time);
	~BenchmarkEngine();

	void AddParticle();
	void AddParticle(const KinematicParameters& parameters);
};