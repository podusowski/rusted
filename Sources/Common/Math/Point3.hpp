#pragma once

#include <cmath>
#include <ostream>

#include "Real.hpp"
#include "Quaternion.hpp"

namespace Common
{
namespace Math
{

template <typename T> class Point3
{
private:
	T m_x;
	T m_y;
	T m_z;

public:
	Point3() : m_x (0), m_y (0), m_z (0) {}

	Point3(T x, T y, T z): m_x(x), m_y(y), m_z(z) {}

	template <typename P> Point3(const Point3<P> p) :
	    m_x(p.getX()), m_y(p.getY()), m_z(p.getZ())
	{
	}

	Point3<T> operator+(const Point3<T> &p)
	{
		return Point3<T>(m_x + p.m_x, m_y + p.m_y, m_z + p.m_z);
	}

	Point3<T>& operator+=(const Point3<T> &p)
	{
		m_x += p.m_x;
		m_y += p.m_y;
		m_z += p.m_z;

		return *this;
	}

	Point3<T> operator-(const Point3<T> &p)
	{
		return Point3<T>(m_x - p.m_x, m_y - p.m_y, m_z - p.m_z);
	}

	Point3<T>& operator-=(const Point3<T> &p)
	{
		m_x -= p.m_x;
		m_y -= p.m_y;
		m_z -= p.m_z;

		return *this;
	}

	bool operator==(const Point3<T> &p) const
	{
		return m_x == p.m_x && m_y == p.m_y && m_z == p.m_z;
	}

	template <typename P> Point3<T> operator*(P value)
	{
		return Point3<T>(ceil(m_x * value), ceil(m_y * value), ceil(m_z * value));
	}

	template <typename P> Point3<T>& operator *= (P value)
	{
		m_x *= value;
		m_y *= value;
		m_z *= value;

		return *this;
	}
	bool operator!=(const Point3<T> &p)
	{
		return m_x != p.m_x || m_y != p.m_y || m_z != p.m_z;
	}

	T getX() const
	{
		return m_x;
	}

	T getY() const
	{
		return m_y;
	}

	T getZ() const
	{
		return m_z;
	}

	void setX(T value)
	{
		m_x = value;
	}

	void setY(T value)
	{
		m_y = value;
	}

	void setZ(T value)
	{
		m_z = value;
	}

	T length()
	{
		return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
	}

	Point3<T> normalize()
	{
        return *this * ( 1.0 / length() );
	}

    Point3<T> crossProduct(const Point3<T> & rhs)
    {
        return Point3<T>(
            m_y * rhs.m_z - m_z * rhs.m_y,
            m_z * rhs.m_x - m_x * rhs.m_z,
            m_x * rhs.m_y - m_y * rhs.m_x);
    }

    Common::Math::Real dotProduct(const Point3<T> & rhs)
    {
        return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
    }

    Quaternion getRotationTo(const Point3<T> & rhs)
    {
        // need higher precision in such calculations
        Point3<Real> p1 = *this;
        Point3<Real> p2 = rhs;

        p1.normalize();
        p2.normalize();

        auto dot = p1.dotProduct(p2);

        if (dot >= 1.0)
        {
            return Quaternion();
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
            return q;
        }
    }

	static T distance(const Point3<T> &a, const Point3<T> &b)
	{
		T sa = a.m_x - b.m_x;
		T sb = a.m_y - b.m_y;
		T sc = a.m_z - b.m_z;

		return sqrt(sa*sa + sb*sb + sc*sc);
	}
};

}
}

template<typename T> std::ostream & operator<<(std::ostream & os, const Common::Math::Point3<T> & p)
{
    os << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
    return os;
}
