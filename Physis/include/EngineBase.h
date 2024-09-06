#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include "Core.h"
#include "ParticleSystem.h"
#include "SystemConfig.h"

class PHYSIS_API EngineBase
{
protected:
	SystemConfig m_config;
	ParticleSystem* m_system;
	std::chrono::duration<double, std::milli> m_duration;
	double m_current_time;

	virtual void OnStartup();
	virtual bool ContinueLoop();
	virtual void OnCompletion();

	virtual void Update(const double& dt) = 0;
	virtual void Render() = 0;
	virtual void Interpolate(const double& factor) = 0;

public:
	EngineBase(const SystemConfig& sc);

	void Run();
	void Pause();
	void Resume();
};