#include "Vec.h"

bool Vec1::operator==(const Vec1& other) const
{
    return X == other.X;
}

bool Vec1::operator!=(const Vec1& other) const
{
    return !(*this == other);
}

bool Vec2::operator==(const Vec2& other) const
{
    return X == other.X && Y == other.Y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return !(*this == other);
}

bool Vec3::operator==(const Vec3& other) const
{
    return X == other.X && Y == other.Y && Z == other.Z;
}

bool Vec3::operator!=(const Vec3& other) const
{
    return !(*this == other);
}
