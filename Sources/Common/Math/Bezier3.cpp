#include <cmath>
#include <boost/math/special_functions/binomial.hpp>

#include "Bezier3.hpp"

using namespace Common::Math;

void Bezier3::addControlPoint(const PointType & point)
{
    m_points.push_back(point);
}

void Bezier3::reset()
{
    m_points.clear();
    m_derivativePoints.clear();
}

bool Bezier3::empty()
{
    return m_points.empty();
}

Bezier3::PointType Bezier3::operator()(float t)
{
    return bezierCurve(m_points, t);
}

Bezier3::PointType Bezier3::derivative(float t)
{
    // http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html

    if (m_derivativePoints.empty())
    {
        calculateDerivativePoints();
    }

    return bezierCurve(m_derivativePoints, t);
}

Quaternion Bezier3::orientation(float t)
{
    return Quaternion();
}

unsigned Bezier3::getLength()
{
    // according to http://www.gamedev.net/topic/313018-calculating-the-length-of-a-bezier-curve/ there
    // is no way to calculate length of the Bezier curve so we need to "measure" it

    const float step = 0.1f;
    float ret = 0;

    for (float i = step; i < 1.0f + step; i += step)
    {
        PointType p1 = (*this)(i - step);
        PointType p2 = (*this)(i);
        ret += PointType::distance(p1, p2);
    }

    return round(ret);
}

Bezier3::PointType Bezier3::bezierCurve(const std::vector<PointType> & points, CalcType t)
{
    if (points.empty())
    {
        throw std::runtime_error("can't calculate bezier curve, no control points");
    }

    CalcType p[3] = {0, 0, 0};
    size_t n = points.size() - 1;

    for (size_t i = 0; i <= n; i++)
    {
        CalcType f = bernsteinPolynomial(i, n, t);

        p[0] += points[i].getX() * f;
        p[1] += points[i].getY() * f;
        p[2] += points[i].getZ() * f;
    }

    return PointType(p[0], p[1], p[2]);
}

Bezier3::CalcType Bezier3::bernsteinPolynomial(unsigned i, unsigned n, CalcType t)
{
    return (CalcType)(boost::math::binomial_coefficient<CalcType>((unsigned int)n, i)*pow(1.0-t, (double)n - i)*pow(t, i));
}

void Bezier3::calculateDerivativePoints()
{
    if (m_points.size() < 2)
    {
        throw std::runtime_error("can't calculate control points for Bezier derivative");
    }

    m_derivativePoints.clear();

    for (unsigned i = 1; i < m_points.size(); i++)
    {
        PointType p = m_points[i] - m_points[i - 1];
        m_derivativePoints.push_back(p);
    }
}

