#include "Point3.hpp"

using namespace Common::Math;

Quaternion Point3::getRotationTo(const Point3 & rhs)
{
    // need higher precision in such calculations
    Point3 p1 = *this;
    Point3 p2 = rhs;

    p1.normalize();
    p2.normalize();

    auto dot = p1.dotProduct(p2);

    if (dot >= 1.0)
    {
        return Quaternion();
    }
    else if (dot <= 1e-6f - 1.0f) // < ~ -1 - negative vectors, totate by 180 degree
    {
        return Quaternion(PI, std::make_tuple(1, 0, 0));
    }
    else
    {
        Real s = sqrt((1 + dot) * 2);
        Real invs = 1 / s;

        auto cross = p1.crossProduct(p2);

        auto x = cross.getX() * invs;
        auto y = cross.getY() * invs;
        auto z = cross.getZ() * invs;
        auto w = s * 0.5f;

        Quaternion q(w, x, y, z);
        q.normalize();

        assert(!std::isnan(q.getW()));
        assert(!std::isnan(q.getX()));
        assert(!std::isnan(q.getY()));
        assert(!std::isnan(q.getZ()));

        return q;
    }
}

