#pragma once

#include "Physis.h"
#include "SystemState.h"

class PHYSIS_API Engine
{
private:
	SystemState* m_system;

public:
	Engine();
	~Engine();

	void Run();
};