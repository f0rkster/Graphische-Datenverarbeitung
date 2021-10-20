#pragma once
class Matrix4x4
{
public:

    Matrix4x4();

    Matrix4x4(float _V00, float _V01, float _V02, float _V03,
        float _V10, float _V11, float _V12, float _V13,
        float _V20, float _V21, float _V22, float _V23,
        float _V30, float _V31, float _V32, float _V33);

    float GetValue(int _X, int _Y);
    
    Matrix4x4  operator +  (Matrix4x4& _Other);
    Matrix4x4& operator += (Matrix4x4& _Other);
    Matrix4x4  operator *  (Matrix4x4& _matrix);
    Matrix4x4& operator *= (Matrix4x4& _matrix);

    void Print();
    
private:
    float m_values[4][4];

};

