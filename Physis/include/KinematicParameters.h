#pragma once

struct KinematicParameters
{
	Vec2 r;
	Vec2 v;
	Vec2 a;

	KinematicParameters() :
		r(Vec2()), v(Vec2()), a(Vec2()) { }

	KinematicParameters(Vec2 r0, Vec2 v0, Vec2 a0) :
		r(r0), v(v0), a(a0) { }
};