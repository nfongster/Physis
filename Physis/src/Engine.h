#pragma once

#ifdef PHYSIS_EXPORTS
#define PHYSIS_API __declspec(dllexport)
#else
#define PHYSIS_API __declspec(dllimport)
#endif

class PHYSIS_API Engine
{
public:
	Engine();
	~Engine();

	void Run();
};