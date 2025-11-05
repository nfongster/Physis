#pragma once

#include <stdlib.h>
#include <corecrt_math.h>
#include <iostream>
#include "Core.h"

const static double DEFAULT_TOLERANCE = 1e-10;

struct PHYSIS_API Vec2
{
public:
	double X;
	double Y;

	Vec2() : X(0), Y(0) {};
	Vec2(double x, double y) : X(x), Y(y) {};

	bool Equals(const Vec2& other, const double& tolerance = DEFAULT_TOLERANCE) const;
	double Dot(const Vec2& other);
	double Magnitude2();
	Vec2 operator+ (const Vec2& other) const;
	Vec2 operator- (const Vec2& other) const;
	Vec2 operator* (const double& scalar) const;
	PHYSIS_API friend std::ostream& operator<<(std::ostream& output, const Vec2& v);
};

struct PHYSIS_API Vec3
{
public:
	double X;
	double Y;
	double Z;

	Vec3() : X(0), Y(0), Z(0) {};
	Vec3(double x, double y, double z) : X(x), Y(y), Z(z) {};

	bool Equals(const Vec3& other, const double& tolerance = DEFAULT_TOLERANCE) const;
	double Dot(const Vec3& other);
	double Magnitude2();
	Vec3 operator+ (const Vec3& other) const;
	Vec3 operator- (const Vec3& other) const;
	Vec3 operator* (const double& scalar) const;
	PHYSIS_API friend std::ostream& operator<<(std::ostream& output, const Vec3& v);
};