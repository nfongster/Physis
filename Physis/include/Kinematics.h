#pragma once

#include "Vec.h"

namespace Kinematics
{
	template <typename V>
	V UpdatePosition(V r0, V v0, V a0, double dt)
	{
		return r0 + (v0 * dt);
	}

	template <typename V>
	V UpdateVelocity(V v0, V a0, double dt)
	{
		return v0 + (a0 * dt);
	}
}