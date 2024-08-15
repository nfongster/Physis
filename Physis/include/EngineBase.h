#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include "Core.h"
#include "ParticleSystem.h"
#include "SystemConfig.h"

class PHYSIS_API EngineBase
{
public:

	virtual void Update(const double& dt) = 0;
	virtual void Render() = 0;
	virtual void Interpolate(const double& factor) = 0;
};