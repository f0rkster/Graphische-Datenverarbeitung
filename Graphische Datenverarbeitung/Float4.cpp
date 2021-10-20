
#include <iostream>
#include "Float4.h"


Float4::Float4()
    :m_values {0}
{
}

Float4::Float4(float _V0, float _V1, float _V2, float _V3)
    :m_values {_V0, _V1, _V2, _V3}
{
}

//Float4(const Float4& other){ /*m_values = other.m_values; */}

float& Float4::operator [](int _Index)
{
    return m_values[_Index];
}

Float4  Float4::operator +  (Float4& _Other) const
{
    Float4 result;

    result[0] = m_values[0] + _Other[0];
    result[1] = m_values[1] + _Other[1];
    result[2] = m_values[2] + _Other[2];
    result[3] = m_values[3] + _Other[3];

    return result;
}

Float4& Float4::operator += (Float4& _Other)
{
    m_values[0] += _Other[0];
    m_values[1] += _Other[1];
    m_values[2] += _Other[2];
    m_values[3] += _Other[3];
    return *this;
}

Float4  Float4::operator -  (Float4& _Other) const
{
    Float4 result;

    result[0] = m_values[0] - _Other[0];
    result[1] = m_values[1] - _Other[1];
    result[2] = m_values[2] - _Other[2];
    result[3] = m_values[3] - _Other[3];

    return result;
}

Float4& Float4::operator -= (Float4& _Other)
{
    m_values[0] -= _Other[0];
    m_values[1] -= _Other[1];
    m_values[2] -= _Other[2];
    m_values[3] -= _Other[3];
    return *this;
}

Float4  Float4::operator *  (float _Other) const
{
    Float4 result;

    result[0] = m_values[0] * _Other;
    result[1] = m_values[1] * _Other;
    result[2] = m_values[2] * _Other;
    result[3] = m_values[3] * _Other;

    return result;
}

Float4& Float4::operator *= (float _Other)
{
    m_values[0] *= _Other;
    m_values[1] *= _Other;
    m_values[2] *= _Other;
    m_values[3] *= _Other;
    return *this;
}


float Float4::Dot(Float4& _Other) const {

    return m_values[0] * _Other[0]
         + m_values[1] * _Other[1]
         + m_values[2] * _Other[2]
         + m_values[3] * _Other[3];
}

Float4 Float4::Cross(Float4& _Other) const {
    Float4 result;
    result[0] = m_values[1] * _Other[2] - m_values[2] * _Other[1];
    result[1] = m_values[2] * _Other[3] - m_values[3] * _Other[2];
    result[2] = m_values[3] * _Other[0] - m_values[0] * _Other[3];
    result[3] = m_values[0] * _Other[1] - m_values[1] * _Other[0];
    return result;
}

Float4& Float4::CrossAndSet(Float4& _Other) {
    float t0 = m_values[1] * _Other[2] - m_values[2] * _Other[1];
    float t1 = m_values[2] * _Other[3] - m_values[3] * _Other[2];
    float t2 = m_values[3] * _Other[0] - m_values[0] * _Other[3];
    float t3 = m_values[0] * _Other[1] - m_values[1] * _Other[0];
    
    m_values[0] = t0;
    m_values[1] = t1;
    m_values[2] = t2;
    m_values[3] = t3;

    return *this;
}

float Float4::GetLength() {
    float result;
    result = sqrt(m_values[0] * m_values[0]
                + m_values[1] * m_values[1]
                + m_values[2] * m_values[2]
                + m_values[3] * m_values[3]);
    return result;
}

Float4 Float4::GetNormalized() {
    return *this * (1 / GetLength());
}

Float4& Float4::Normalize() {
    return *this *= (1 / GetLength());
}

void Float4::Print() {
    for (float value : Float4::m_values)
    {
        std::cout << value << std::endl;
    }
}
