#pragma once

#include "Types.hpp"
#include <cmath>
#include <ostream>

namespace Common
{

template <typename T=int>
class Point3
{
private:
	T x;
	T y;
	T z;

public:
	Point3() : x (0), y (0), z (0) {}

	Point3(T x, T y, T z): x (x), y (y), z (z) {}

	template <typename P> Point3(const Point3<P> p) :
	    x(p.getX()), y(p.getY()), z(p.getZ())
	{
	}

	Point3<T> operator+(const Point3<T> &p)
	{
		return Point3<T>(x + p.x, y + p.y, z + p.z);
	}

	Point3<T>& operator+=(const Point3<T> &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;

		return *this;
	}

	Point3<T> operator-(const Point3<T> &p)
	{
		return Point3<T>(x - p.x, y - p.y, z - p.z);
	}

	Point3<T>& operator-=(const Point3<T> &p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;

		return *this;
	}

	bool operator==(const Point3<T> &p) const
	{
		return x == p.x && y == p.y && z == p.z;
	}

	template <typename P> Point3<T> operator*(P value)
	{
		return Point3<T>(x * value, y * value, z * value);
	}

	template <typename P> Point3<T>& operator *= (P value)
	{
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}
	bool operator!=(const Point3<T> &p)
	{
		return x != p.x || y != p.y || z != p.z;
	}

	T getX() const
	{
		return x;
	}

	T getY() const
	{
		return y;
	}

	T getZ() const
	{
		return z;
	}

	void setX(T value)
	{
		x = value;
	}

	void setY(T value)
	{
		y = value;
	}

	void setZ(T value)
	{
		z = value;
	}

	T length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	Point3<T> normalize()
	{
        return *this * ( 1.0 / length() );
	}

	static T distance(const Point3<T> &a, const Point3<T> &b)
	{
		T sa = a.x - b.x;
		T sb = a.y - b.y;
		T sc = a.z - b.z;

		return sqrt(sa*sa + sb*sb + sc*sc);
	}
};

}

template<typename T>
std::ostream & operator<<(std::ostream & os, const Common::Point3<T> & p)
{
    os << "x: " << p.getX() << ", y: " << p.getY() << ", z: " << p.getZ();
    return os;
}
