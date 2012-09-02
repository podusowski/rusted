#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "Point3.hpp"
#include <vector>
#include <boost/math/special_functions/binomial.hpp>
#include <cmath>

namespace Common
{

/**
 * Generic implementation for bezier curves.
 */
template<typename Point, typename CalcType = float> class Bezier
{
	std::vector<Point> points;

public:
	/**
	 * Adds control point for bezier curve. 
	 */
	void addControlPoint(const Point &point)
	{
		points.push_back(point);
	}

	/**
	 * Returns value for bezier function with t parameter.
	 */
	Point operator()(float t)
	{
		CalcType f = 0;
		CalcType p[3] = {0, 0, 0};
		size_t n = points.size() - 1;

		for (int i = 0; i <= n; i ++)
		{
			f = (CalcType)(boost::math::binomial_coefficient<CalcType>((unsigned int)n, i)*pow(1.0-t, (double)n - i)*pow(t, i));

			p[0] += points[i].getX() * f;
			p[1] += points[i].getY() * f;
			p[2] += points[i].getZ() * f;
		}

		return Point(p[0], p[1], p[2]);
	}
};

}

#endif

