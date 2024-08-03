#pragma once

struct SystemConfig
{
	double total_time;
	double delta_time;

	SystemConfig() :
		total_time(0), delta_time(0) { }

	SystemConfig(double t_total, double dt) :
		total_time(t_total), delta_time(dt) { }
};