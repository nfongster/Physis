#include "Vec.h"

bool Vec1::operator==(const Vec1& other) const
{
    return X == other.X;
}

bool Vec1::operator!=(const Vec1& other) const
{
    return !(*this == other);
}

Vec1 Vec1::operator+(const Vec1& other) const
{
    return Vec1(this->X + other.X);
}

Vec1 Vec1::operator-(const Vec1& other) const
{
    return Vec1(this->X - other.X);
}

Vec1 Vec1::operator*(const double scalar) const
{
    return Vec1(this->X * scalar);
}

bool Vec2::operator==(const Vec2& other) const
{
    return X == other.X && Y == other.Y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return !(*this == other);
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(this->X + other.X, this->Y + other.Y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(this->X - other.X, this->Y - other.Y);
}

Vec2 Vec2::operator*(const double scalar) const
{
    return Vec2(this->X * scalar, this->Y * scalar);
}

bool Vec3::operator==(const Vec3& other) const
{
    return X == other.X && Y == other.Y && Z == other.Z;
}

bool Vec3::operator!=(const Vec3& other) const
{
    return !(*this == other);
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(this->X + other.X, this->Y + other.Y, this->Z + other.Z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(this->X - other.X, this->Y - other.Y, this->Z - other.Z);
}

Vec3 Vec3::operator*(const double scalar) const
{
    return Vec3(this->X * scalar, this->Y * scalar, this->Z * scalar);
}
