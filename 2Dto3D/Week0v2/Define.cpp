#include "Define.h"

// ���� ��� ����
const FMatrix FMatrix::Identity = { {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
} };

// ��� ����
FMatrix FMatrix::operator+(const FMatrix& Other) const {
    FMatrix Result;
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] + Other.M[i][j];
    return Result;
}

// ��� ����
FMatrix FMatrix::operator-(const FMatrix& Other) const {
    FMatrix Result;
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] - Other.M[i][j];
    return Result;
}

// ��� ����
FMatrix FMatrix::operator*(const FMatrix& Other) const {
    FMatrix Result = {};
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            for (int32 k = 0; k < 4; k++)
                Result.M[i][j] += M[i][k] * Other.M[k][j];
    return Result;
}

// ��Į�� ����
FMatrix FMatrix::operator*(float Scalar) const {
    FMatrix Result;
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] * Scalar;
    return Result;
}

// ��Į�� ������
FMatrix FMatrix::operator/(float Scalar) const {
    FMatrix Result;
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] / Scalar;
    return Result;
}

float* FMatrix::operator[](int row) {
    return M[row];
}

const float* FMatrix::operator[](int row) const
{
    return M[row];
}

FVector FMatrix::GetScale() const
{
    return FVector(
        sqrt(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]), // X�� ���� ����
        sqrt(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]), // Y�� ���� ����
        sqrt(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2])  // Z�� ���� ����
    );
}


// ��ġ ���
FMatrix FMatrix::Transpose(const FMatrix& Mat) {
    FMatrix Result;
    for (int32 i = 0; i < 4; i++)
        for (int32 j = 0; j < 4; j++)
            Result.M[i][j] = Mat.M[j][i];
    return Result;
}

// ��Ľ� ��� (���ö� ����, 4x4 ���)
float FMatrix::Determinant(const FMatrix& Mat) {
    float det = 0.0f;
    for (int32 i = 0; i < 4; i++) {
        float subMat[3][3];
        for (int32 j = 1; j < 4; j++) {
            int32 colIndex = 0;
            for (int32 k = 0; k < 4; k++) {
                if (k == i) continue;
                subMat[j - 1][colIndex] = Mat.M[j][k];
                colIndex++;
            }
        }
        float minorDet =
            subMat[0][0] * (subMat[1][1] * subMat[2][2] - subMat[1][2] * subMat[2][1]) -
            subMat[0][1] * (subMat[1][0] * subMat[2][2] - subMat[1][2] * subMat[2][0]) +
            subMat[0][2] * (subMat[1][0] * subMat[2][1] - subMat[1][1] * subMat[2][0]);
        det += (i % 2 == 0 ? 1 : -1) * Mat.M[0][i] * minorDet;
    }
    return det;
}

// ����� (���콺-���� �ҰŹ�)
FMatrix FMatrix::Inverse(const FMatrix& Mat) {
    float det = Determinant(Mat);
    if (fabs(det) < 1e-6) {
        return Identity;
    }

    FMatrix Inv;
    float invDet = 1.0f / det;

    // ���μ� ��� ��� �� ��ġ�Ͽ� ����� ���
    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 4; j++) {
            float subMat[3][3];
            int32 subRow = 0;
            for (int32 r = 0; r < 4; r++) {
                if (r == i) continue;
                int32 subCol = 0;
                for (int32 c = 0; c < 4; c++) {
                    if (c == j) continue;
                    subMat[subRow][subCol] = Mat.M[r][c];
                    subCol++;
                }
                subRow++;
            }
            float minorDet =
                subMat[0][0] * (subMat[1][1] * subMat[2][2] - subMat[1][2] * subMat[2][1]) -
                subMat[0][1] * (subMat[1][0] * subMat[2][2] - subMat[1][2] * subMat[2][0]) +
                subMat[0][2] * (subMat[1][0] * subMat[2][1] - subMat[1][1] * subMat[2][0]);

            Inv.M[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * minorDet * invDet;
        }
    }
    return Inv;
}

FMatrix FMatrix::CreateRotation(float roll, float pitch, float yaw)
{
    float radRoll = roll * (3.14159265359f / 180.0f);
    float radPitch = pitch * (3.14159265359f / 180.0f);
    float radYaw = yaw * (3.14159265359f / 180.0f);

    float cosRoll = cos(radRoll), sinRoll = sin(radRoll);
    float cosPitch = cos(radPitch), sinPitch = sin(radPitch);
    float cosYaw = cos(radYaw), sinYaw = sin(radYaw);

    // Z�� (Yaw) ȸ��
    FMatrix rotationZ = { {
        { cosYaw, sinYaw, 0, 0 },
        { -sinYaw, cosYaw, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    } };

    // Y�� (Pitch) ȸ��
    FMatrix rotationY = { {
        { cosPitch, 0, -sinPitch, 0 },
        { 0, 1, 0, 0 },
        { sinPitch, 0, cosPitch, 0 },
        { 0, 0, 0, 1 }
    } };

    // X�� (Roll) ȸ��
    FMatrix rotationX = { {
        { 1, 0, 0, 0 },
        { 0, cosRoll, sinRoll, 0 },
        { 0, -sinRoll, cosRoll, 0 },
        { 0, 0, 0, 1 }
    } };

    // DirectX ǥ�� ����: Z(Yaw) �� Y(Pitch) �� X(Roll)  
    return rotationX * rotationY * rotationZ;  // �̷��� �ϸ�  ������ ���� �����
}


// ������ ��� ����
FMatrix FMatrix::CreateScale(float scaleX, float scaleY, float scaleZ)
{
    return { {
        { scaleX, 0, 0, 0 },
        { 0, scaleY, 0, 0 },
        { 0, 0, scaleZ, 0 },
        { 0, 0, 0, 1 }
    } };
}

FMatrix FMatrix::CreateTranslationMatrix(const FVector& position)
{
    FMatrix translationMatrix = FMatrix::Identity;
    translationMatrix.M[3][0] = position.x;
    translationMatrix.M[3][1] = position.y;
    translationMatrix.M[3][2] = position.z;
    return translationMatrix;
}

FVector FMatrix::TransformVector(const FVector& v, const FMatrix& m)
{
    FVector result;

    // 4x4 ����� ����Ͽ� ���� ��ȯ (W = 0���� ����, ���� ����)
    result.x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + 0.0f * m.M[3][0];
    result.y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + 0.0f * m.M[3][1];
    result.z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + 0.0f * m.M[3][2];


    return result;
}

// FVector4�� ��ȯ�ϴ� �Լ�
FVector4 FMatrix::TransformVector(const FVector4& v, const FMatrix& m)
{
    FVector4 result;
    result.x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + v.a * m.M[3][0];
    result.y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + v.a * m.M[3][1];
    result.z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + v.a * m.M[3][2];
    result.a = v.x * m.M[0][3] + v.y * m.M[1][3] + v.z * m.M[2][3] + v.a * m.M[3][3];
    return result;
}


