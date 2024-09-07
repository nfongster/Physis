#pragma once

struct TimeConfig
{
	double total_time;  // simulation time
	double delta_time;  // simulation time
	double time_scalar; // simulation time / computational time

	TimeConfig() :
		total_time(0), delta_time(0), time_scalar(0) { }

	TimeConfig(const double& t_total, const double& dt, const double& scalar) :
		total_time(t_total), delta_time(dt), time_scalar(scalar) { }
};