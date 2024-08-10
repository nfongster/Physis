#include "Vec.h"

bool Vec1::Equals(const Vec1& other, const double& tolerance) const
{
    return fabs(this->X - other.X) <= tolerance;
}

Vec1 Vec1::operator+(const Vec1& other) const
{
    return Vec1(this->X + other.X);
}

Vec1 Vec1::operator-(const Vec1& other) const
{
    return Vec1(this->X - other.X);
}

Vec1 Vec1::operator*(const double& scalar) const
{
    return Vec1(this->X * scalar);
}

std::ostream& operator<<(std::ostream& output, const Vec1& v)
{
    output << "(" << v.X << ")";
    return output;
}

bool Vec2::Equals(const Vec2& other, const double& tolerance) const
{
    return fabs(this->X - other.X) <= tolerance 
        && fabs(this->Y - other.Y) <= tolerance;
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(this->X + other.X, this->Y + other.Y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(this->X - other.X, this->Y - other.Y);
}

Vec2 Vec2::operator*(const double& scalar) const
{
    return Vec2(this->X * scalar, this->Y * scalar);
}

std::ostream& operator<<(std::ostream& output, const Vec2& v)
{
    output << "(" << v.X << ", " << v.Y << ")";
    return output;
}

bool Vec3::Equals(const Vec3& other, const double& tolerance) const
{
    return fabs(this->X - other.X) <= tolerance 
        && fabs(this->Y - other.Y) <= tolerance 
        && fabs(this->Z - other.Z) <= tolerance;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(this->X + other.X, this->Y + other.Y, this->Z + other.Z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(this->X - other.X, this->Y - other.Y, this->Z - other.Z);
}

Vec3 Vec3::operator*(const double& scalar) const
{
    return Vec3(this->X * scalar, this->Y * scalar, this->Z * scalar);
}

std::ostream& operator<<(std::ostream& output, const Vec3& v)
{
    output << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return output;
}