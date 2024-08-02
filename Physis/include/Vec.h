#pragma once

struct Vec1
{
public:
	double X;

	Vec1() : X(0) {};
	Vec1(double x) : X(x) {};
};

struct Vec2
{
public:
	double X;
	double Y;

	Vec2() : X(0), Y(0) {};
	Vec2(double x, double y) : X(x), Y(y) {};
};

struct Vec3
{
public:
	double X;
	double Y;
	double Z;

	Vec3() : X(0), Y(0), Z(0) {};
	Vec3(double x, double y, double z) : X(x), Y(y), Z(z) {};
};