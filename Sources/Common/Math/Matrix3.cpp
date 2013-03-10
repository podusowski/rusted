#include "Matrix3.hpp"

using namespace Common::Math;

Matrix3::Matrix3()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m_data[i][j] = 0;
        }
    }
}

Matrix3::Matrix3(
    Real x00, Real x01, Real x02,
    Real x10, Real x11, Real x12,
    Real x20, Real x21, Real x22)
{
    m_data[0][0] = x00;
    m_data[0][1] = x01;
    m_data[0][2] = x02;
    m_data[1][0] = x10;
    m_data[1][1] = x11;
    m_data[1][2] = x12;
    m_data[2][0] = x20;
    m_data[2][1] = x21;
    m_data[2][2] = x22;
}

Real & Matrix3::operator () (size_t row, size_t column)
{
    return m_data[row][column];
}

