#pragma once

#include <cstddef>

#include "Real.hpp"

namespace Common
{
namespace Math
{

/*
 * 00 01 02
 * 10 11 12
 * 20 21 22
 */

class Matrix3
{
public:
    Matrix3();

    Matrix3(
        Real x00, Real x01, Real x02,
        Real x10, Real x11, Real x12,
        Real x20, Real x21, Real x22);

    Real & operator () (size_t row, size_t column);
    Real operator () (size_t row, size_t column) const;

private:
    Real m_data[3][3];
};

}
}

