#pragma once

#ifdef PHYSIS_EXPORTS
#define PHYSIS_API __declspec(dllexport)
#else
#define PHYSIS_API __declspec(dllimport)
#endif

#include "Engine.h"