#pragma once

#include <iostream>
#include <thread>
#include "Core.h"
#include "ParticleSystem.h"
#include "SystemConfig.h"

class PHYSIS_API Engine
{
private:
	SystemConfig m_config;
	ParticleSystem* m_system;

public:
	Engine(const SystemConfig& sc);
	~Engine();

	void Run();
	void Pause();
	void Resume();

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
	std::vector<Particle*> Sample();
};