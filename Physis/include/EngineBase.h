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
	std::shared_ptr<SystemState> m_system_state;

	std::chrono::duration<double, std::milli> m_duration;
	double m_current_time;

	virtual void OnStartup();
	virtual bool ContinueLoop();
	virtual void OnCompletion();

	virtual void Update(const double& dt);
	virtual void Render() = 0;
	virtual void Interpolate(const double& factor);

public:
	EngineBase(const TimeConfig& config);

	void Run();
	void Pause();
	void Resume();
};