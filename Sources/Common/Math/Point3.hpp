#pragma once

#include <cmath>
#include <ostream>

#include "Real.hpp"
#include "Quaternion.hpp"

namespace Common
{
namespace Math
{

class Quaternion;

class Point3
{
public:
    typedef Real ScalarType;

private:
	ScalarType m_x;
	ScalarType m_y;
	ScalarType m_z;

public:
	Point3() : m_x (0), m_y (0), m_z (0) {}

	Point3(ScalarType x, ScalarType y, ScalarType z):
	    m_x(x), m_y(y), m_z(z)
	{
	}

	Point3 operator+(const Point3 & p) const
	{
		return Point3(m_x + p.m_x, m_y + p.m_y, m_z + p.m_z);
	}

	Point3 & operator+=(const Point3 & p)
	{
		m_x += p.m_x;
		m_y += p.m_y;
		m_z += p.m_z;

		return *this;
	}

	Point3 operator-(const Point3 & p) const
	{
		return Point3(m_x - p.m_x, m_y - p.m_y, m_z - p.m_z);
	}

	Point3 & operator-=(const Point3 & p)
	{
		m_x -= p.m_x;
		m_y -= p.m_y;
		m_z -= p.m_z;

		return *this;
	}

	bool operator==(const Point3 & p) const
	{
		return m_x == p.m_x && m_y == p.m_y && m_z == p.m_z;
	}

	template <typename P> Point3 operator*(P value) const
	{
		return Point3(ceil(m_x * value), ceil(m_y * value), ceil(m_z * value));
	}

	Point3 & operator *= (Real value)
	{
		m_x *= value;
		m_y *= value;
		m_z *= value;

		return *this;
	}

	bool operator!=(const Point3 &p) const
	{
		return m_x != p.m_x || m_y != p.m_y || m_z != p.m_z;
	}

	ScalarType getX() const
	{
		return m_x;
	}

	ScalarType getY() const
	{
		return m_y;
	}

	ScalarType getZ() const
	{
		return m_z;
	}

	void setX(ScalarType value)
	{
		m_x = value;
	}

	void setY(ScalarType value)
	{
		m_y = value;
	}

	void setZ(ScalarType value)
	{
		m_z = value;
	}

	ScalarType length()
	{
		return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
	}

	void normalize()
	{
        *this *= ( 1.0 / length() );
	}

    Point3 crossProduct(const Point3 & rhs)
    {
        return Point3(
            m_y * rhs.m_z - m_z * rhs.m_y,
            m_z * rhs.m_x - m_x * rhs.m_z,
            m_x * rhs.m_y - m_y * rhs.m_x);
    }

    Common::Math::Real dotProduct(const Point3 & rhs)
    {
        return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
    }

    Quaternion getRotationTo(const Point3 & rhs);
    
    static Real distance(const Point3 & a, const Point3 & b)
    {
        ScalarType sa = a.m_x - b.m_x;
        ScalarType sb = a.m_y - b.m_y;
        ScalarType sc = a.m_z - b.m_z;

        return sqrt(sa*sa + sb*sb + sc*sc);
    }
};

inline std::ostream & operator<<(std::ostream & os, const Common::Math::Point3 & p)
{
    os << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
    return os;
}

}
}
