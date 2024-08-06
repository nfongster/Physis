#pragma once

#include <iostream>
#include "Core.h"
#include "ParticleSystem.h"

class PHYSIS_API Engine
{
private:
	ParticleSystem* m_system;

public:
	Engine();
	~Engine();

	void Run();
};