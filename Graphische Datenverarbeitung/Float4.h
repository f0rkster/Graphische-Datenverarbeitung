#pragma once

class Float4 {
public:
    float m_values[4];

    Float4();

    Float4(float _value1, float _value2, float _value3, float _value4);

    //Float4(const Float4& other);

    float Get(int _index);

    Float4& Add(Float4& _vector);

    void AddAndSet(Float4& _vector);

    Float4& Sub(Float4& _vector);

    void SubAndSet(Float4& _vector);

    Float4& Mul(Float4& _vector);

    void MulAndSet(Float4& _vector);

    float Dot(Float4& _vector);

    Float4& Cross(Float4& _vector);

    void CrossAndSet(Float4& _vector);

    float GetLength();

    Float4& GetNormalized();

    void Normalize();

    void Print();
};