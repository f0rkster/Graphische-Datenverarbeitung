#pragma once

class Float4 {
public:

    Float4();
    Float4(float _V0, float _V1, float _V2, float _V3);
    float&  operator [] (int _Index);
    Float4  operator +  (Float4& _Other) const;
    Float4& operator += (Float4& _Other);
    Float4  operator -  (Float4& _Other) const;
    Float4& operator -= (Float4& _Other);
    Float4  operator *  (float _Other) const;
    Float4& operator *= (float _Other);
    float Dot(Float4& _Other) const;
    Float4 Cross(Float4& _Other) const;
    Float4& CrossAndSet(Float4& _Other);
    float GetLength();
    Float4 GetNormalized();
    Float4& Normalize();
    void Print();

private:
    float m_values[4];

};