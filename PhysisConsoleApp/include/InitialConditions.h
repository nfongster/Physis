#pragma once
#include <Engine.h>

struct InitialConditions
{
	Vec2 r;
	Vec2 v;
	Vec2 a;

	InitialConditions() :
		r(Vec2()), v(Vec2()), a(Vec2()) { }

	InitialConditions(Vec2 r0, Vec2 v0, Vec2 a0) :
		r(r0), v(v0), a(a0) { }
};