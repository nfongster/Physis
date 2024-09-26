#pragma once

struct TimeConfig
{
	std::chrono::duration<double> total_time;  // simulation time
	std::chrono::duration<double> delta_time;  // simulation time
	double time_scalar; // simulation time / computational time

	TimeConfig() :
		total_time(0), delta_time(0), time_scalar(0) { }

	TimeConfig(const std::chrono::duration<double>& t_total, 
			   const std::chrono::duration<double>& dt, 
			   const double& scalar) :
		total_time(t_total), delta_time(dt), time_scalar(scalar) { }
};