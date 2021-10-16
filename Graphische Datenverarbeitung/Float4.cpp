
#include <iostream>
#include "Float4.h"


Float4::Float4() {
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        m_values[i]=0;
    }
}

Float4::Float4(float _value1, float _value2, float _value3, float _value4)
{
    m_values[0] = _value1;
    m_values[1] = _value2;
    m_values[2] = _value3;
    m_values[3] = _value4;
}

//Float4(const Float4& other){ /*m_values = other.m_values; */}

float Float4::Get(int _index) { return m_values[_index]; }

Float4& Float4::Add(Float4& _vector)
{
    Float4 result;
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        result.m_values[i] = m_values[i] + _vector.m_values[i];
    }
    return result;
}

void Float4::AddAndSet(Float4& _vector) {
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        m_values[i] = m_values[i] + _vector.m_values[i];
    }
}

Float4& Float4::Sub(Float4& _vector)
{
    Float4 result;
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        result.m_values[i] = m_values[i] - _vector.m_values[i];
    }
    return result;
}

void Float4::SubAndSet(Float4& _vector) {
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        m_values[i] = m_values[i] - _vector.m_values[i];
    }
}

Float4& Float4::Mul(Float4& _vector)
{
    Float4 result;
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        result.m_values[i] = m_values[i] * _vector.m_values[i];
    }
    return result;
}

void Float4::MulAndSet(Float4& _vector) {
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        m_values[i] = m_values[i] * _vector.m_values[i];
    }
}

float Float4::Dot(Float4& _vector) {
    float result;
    result = m_values[0] * _vector.m_values[0]
        + m_values[1] * _vector.m_values[1] + m_values[2] * _vector.m_values[2]
        + m_values[3] * _vector.m_values[3];
    return result;
}

Float4& Float4::Cross(Float4& _vector) {
    Float4 result;
    result.m_values[0] = m_values[2] * _vector.m_values[3] - m_values[3] * _vector.m_values[2];
    result.m_values[1] = m_values[3] * _vector.m_values[4] - m_values[4] * _vector.m_values[3];
    result.m_values[2] = m_values[4] * _vector.m_values[1] - m_values[1] * _vector.m_values[4];
    result.m_values[3] = m_values[1] * _vector.m_values[2] - m_values[2] * _vector.m_values[1];
    return result;
}

void Float4::CrossAndSet(Float4& _vector) {
    m_values[0] = m_values[2] * _vector.m_values[3] - m_values[3] * _vector.m_values[2];
    m_values[1] = m_values[3] * _vector.m_values[4] - m_values[4] * _vector.m_values[3];
    m_values[2] = m_values[4] * _vector.m_values[1] - m_values[1] * _vector.m_values[4];
    m_values[3] = m_values[1] * _vector.m_values[2] - m_values[2] * _vector.m_values[1];
}

float Float4::GetLength() {
    float result;
    result = sqrt(m_values[0] * m_values[0]
        + m_values[1] * m_values[1]
        + m_values[2] * m_values[2]
        + m_values[3] * m_values[3]);
    return result;
}

Float4& Float4::GetNormalized() {
    float length = this->GetLength();
    Float4 result;
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        result.m_values[i] = m_values[i] / length;
    }
    return result;
}

void Float4::Normalize() {
    float length = this->GetLength();
    int m_values_length = sizeof(m_values) / sizeof(*m_values);
    for (int i = 0; i < m_values_length; i++)
    {
        m_values[i] = m_values[i] / length;
    }
}

void Float4::Print() {
    for (float value : Float4::m_values)
    {
        std::cout << value << std::endl;
    }
}
