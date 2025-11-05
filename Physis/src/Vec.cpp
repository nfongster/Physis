#include "Vec.h"

bool Vec2::Equals(const Vec2& other, const double& tolerance) const
{
    return fabs(this->X - other.X) <= tolerance 
        && fabs(this->Y - other.Y) <= tolerance;
}

double Vec2::Dot(const Vec2& other)
{
    return (this->X * other.X) + (this->Y * other.Y);
}

double Vec2::Magnitude2()
{
    return (this->X * this->X) + (this->Y * this->Y);
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

double Vec3::Dot(const Vec3& other)
{
    return (this->X * other.X) + (this->Y * other.Y) + (this->Z * other.Z);
}

double Vec3::Magnitude2()
{
    return (this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z);
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