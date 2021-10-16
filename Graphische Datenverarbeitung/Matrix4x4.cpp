#include "Matrix4x4.h"

#include <iostream>

Matrix4x4::Matrix4x4()
{
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            m_values[row][col] = 0;
        }
    }
}

Matrix4x4::Matrix4x4(float v11, float v12, float v13, float v14, 
                    float v21, float v22, float v23, float v24, 
                    float v31, float v32, float v33, float v34, 
                    float v41, float v42, float v43, float v44)
{
    m_values[0][0] = v11;
    m_values[0][1] = v12;
    m_values[0][2] = v13;
    m_values[0][3] = v14;
    m_values[1][0] = v21;
    m_values[1][1] = v22;
    m_values[1][2] = v23;
    m_values[1][3] = v24;
    m_values[2][0] = v31;
    m_values[2][1] = v32;
    m_values[2][2] = v33;
    m_values[2][3] = v34;
    m_values[3][0] = v41;
    m_values[3][1] = v42;
    m_values[3][2] = v43;
    m_values[3][3] = v44;
}

Matrix4x4& Matrix4x4::Add(Matrix4x4& _matrix)
{
    Matrix4x4 result;
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            result.m_values[row][col] = m_values[row][col] + _matrix.m_values[row][col];
        }
    }
    return result;
}

void Matrix4x4::AddAndSet(Matrix4x4& _matrix)
{
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            m_values[row][col] = m_values[row][col] + _matrix.m_values[row][col];
        }
    }
}

Matrix4x4& Matrix4x4::Mul(Matrix4x4& _matrix)
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

void Matrix4x4::MulAndSet(Matrix4x4& _matrix)
{
    int m_values_length = 4;
    for (int row = 0; row < m_values_length; row++)
    {
        for (int col = 0; col < m_values_length; col++)
        {
            m_values[row][col] = m_values[row][col] * _matrix.m_values[row][col];
        }
    }
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

