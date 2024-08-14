#pragma once

struct SystemConfig
{
	double total_time;  // simulation time
	double delta_time;  // simulation time
	double time_scalar; // simulation time / computational time

	SystemConfig() :
		total_time(0), delta_time(0), time_scalar(0) { }

	SystemConfig(double t_total, double dt, double scalar) :
		total_time(t_total), delta_time(dt), time_scalar(scalar) { }
};