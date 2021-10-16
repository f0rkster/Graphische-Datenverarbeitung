#pragma once
class Matrix4x4
{
public:
    float m_values[4][4];

    Matrix4x4();

    Matrix4x4(float v11, float v12, float v13, float v14,
        float v21, float v22, float v23, float v24,
        float v31, float v32, float v33, float v34,
        float v41, float v42, float v43, float v44);

    Matrix4x4& Add(Matrix4x4& _matrix);

    void AddAndSet(Matrix4x4& _matrix);

    Matrix4x4& Mul(Matrix4x4& _matrix);

    void MulAndSet(Matrix4x4& _matrix);

    void Print();
    
};

