#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include "Core.h"
#include "ParticleSystem.h"
#include "TimeConfig.h"
#include "SystemState.h"

class PHYSIS_API EngineBase
{
protected:
	TimeConfig m_config;
	std::chrono::duration<double> m_current_frame_time;
	std::chrono::duration<double> m_elapsed_simulation_time;
	std::shared_ptr<SystemState> m_system_state;

	virtual void OnStartup();
	virtual bool ContinueLoop();
	virtual void OnCompletion();

	virtual void Update(const double& dt_sec);
	virtual void Render() = 0;
	virtual void Interpolate(const double& factor);

public:
	EngineBase(const TimeConfig& config);

	void Run();
	void Pause();
	void Resume();
};