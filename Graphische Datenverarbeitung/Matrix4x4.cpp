#include "Matrix4x4.h"

#include <iostream>

Matrix4x4::Matrix4x4()
    :m_values{0}
{
}

Matrix4x4::Matrix4x4(float _V00, float _V01, float _V02, float _V03,
                    float _V10, float _V11, float _V12, float _V13,
                    float _V20, float _V21, float _V22, float _V23,
                    float _V30, float _V31, float _V32, float _V33)
    : m_values{ {_V00, _V01, _V02, _V03}
              , {_V10, _V11, _V12, _V13}
              , {_V20, _V21, _V22, _V23}
              , {_V30, _V31, _V32, _V33} }
{
}

float Matrix4x4::GetValue(int _X, int _Y)
{
    return m_values[_X][_Y];
}

Matrix4x4 Matrix4x4::operator + (Matrix4x4& _Other)
{
    Matrix4x4 result;
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            result.m_values[row][col] = m_values[row][col] + _Other.m_values[row][col];
        }
    }
    return result;
}

Matrix4x4& Matrix4x4::operator +=(Matrix4x4& _Other)
{
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            m_values[row][col] = m_values[row][col] + _Other.m_values[row][col];
        }
    }
    return *this;
}

Matrix4x4 Matrix4x4::operator * (Matrix4x4& _matrix)
{
    Matrix4x4 result;
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            result.m_values[row][col] = m_values[row][col] * _matrix.m_values[row][col];
        }
    }
    return result;
}

Matrix4x4& Matrix4x4::operator *= (Matrix4x4& _matrix)
{
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            m_values[row][col] = m_values[row][col] * _matrix.m_values[row][col];
        }
    }
    return *this;
}

void Matrix4x4::Print() {
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            std::cout << m_values[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

