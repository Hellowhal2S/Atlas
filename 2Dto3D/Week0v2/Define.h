#pragma once
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#define PI 3.141592
#pragma once

#define TArray std::vector
#define TDoubleLinkedList std::list
#define UE_LOG Console::GetInstance().AddLog
#define Super __super
#define TMap std::unordered_map

#define DECLARE_CLASS(ClassName, ParentClassName) \
public: \
    static UClass* StaticClass() { \
        static UClass ClassInfo(#ClassName, ParentClassName::StaticClass()); \
        return &ClassInfo; \
    } \
    virtual UClass* GetClass() const override { return StaticClass(); }

using int32 = int;
using uint32 = unsigned int;
using FString = std::string;
using FWString = std::wstring;

#include "Console.h"

struct FVertexSimple
{
	float x, y, z;    // Position
	float r, g, b, a; // Color
};
struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	FVector operator-(const FVector& other) const {
		return FVector(x - other.x, y - other.y, z - other.z);
	}
	FVector operator+(const FVector& other) const {
		return FVector(x + other.x, y + other.y, z + other.z);
	}

	// ���� ����
	float Dot(const FVector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// ���� ũ��
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	// ���� ����ȭ
	FVector Normalize() const {
		float mag = Magnitude();
		return (mag > 0) ? FVector(x / mag, y / mag, z / mag) : FVector(0, 0, 0);
	}
	FVector Cross(const FVector& Other) const
	{
		return FVector{
			y * Other.z - z * Other.y,
			z * Other.x - x * Other.z,
			x * Other.y - y * Other.x
		};
	}
	// ��Į�� ����
	FVector operator*(float scalar) const {
		return FVector(x * scalar, y * scalar, z * scalar);
	}

	bool operator==(const FVector& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	float Distance(const FVector& other) const {
		// �� ������ �� ������ ũ�⸦ ���
		return ((*this - other).Magnitude());
	}
	DirectX::XMFLOAT3 ToXMFLOAT3() const
	{
		return DirectX::XMFLOAT3(x, y, z);
	}

};
struct FVector4 {
	float x, y, z, a;
	FVector4(float _x = 0, float _y = 0, float _z = 0, float _a = 0) : x(_x), y(_y), z(_z), a(_a) {}

	FVector4 operator-(const FVector4& other) const {
		return FVector4(x - other.x, y - other.y, z - other.z, a - other.a);
	}
	FVector4 operator+(const FVector4& other) const {
		return FVector4(x + other.x, y + other.y, z + other.z, a + other.a);
	}
	FVector4 operator/(float scalar) const
	{
		return FVector4{ x / scalar, y / scalar, z / scalar, a / scalar };
	}
};
struct FMatrix
{
	float M[4][4];
	static const FMatrix Identity;
	// �⺻ ������ �����ε�
	FMatrix operator+(const FMatrix& Other) const;
	FMatrix operator-(const FMatrix& Other) const;
	FMatrix operator*(const FMatrix& Other) const;
	FMatrix operator*(float Scalar) const;
	FMatrix operator/(float Scalar) const;
	float* operator[](int row);
	const float* operator[](int row) const;
	// ��ƿ��Ƽ �Լ�
	static FMatrix Transpose(const FMatrix& Mat);
	static float Determinant(const FMatrix& Mat);
	static FMatrix Inverse(const FMatrix& Mat);
	static FMatrix CreateRotation(float roll, float pitch, float yaw);
	static FMatrix CreateScale(float scaleX, float scaleY, float scaleZ);
	static FVector TransformVector(const FVector& v, const FMatrix& m);
	static FVector4 TransformVector(const FVector4& v, const FMatrix& m);
	static FMatrix CreateTranslationMatrix(const FVector& position);


	DirectX::XMMATRIX ToXMMATRIX() const
	{
		return DirectX::XMMatrixSet(
			M[0][0], M[1][0], M[2][0], M[3][0], // ù ��° ��
			M[0][1], M[1][1], M[2][1], M[3][1], // �� ��° ��
			M[0][2], M[1][2], M[2][2], M[3][2], // �� ��° ��
			M[0][3], M[1][3], M[2][3], M[3][3]  // �� ��° ��
		);
	}
	FVector4 TransformFVector4(const FVector4& vector)
	{
		return FVector4(
			M[0][0] * vector.x +M[1][0] * vector.y + M[2][0] * vector.z + M[3][0] * vector.a,
			M[0][1] * vector.x +M[1][1] * vector.y + M[2][1] * vector.z + M[3][1] * vector.a,
			M[0][2] * vector.x +M[1][2] * vector.y + M[2][2] * vector.z + M[3][2] * vector.a,
			M[0][3] * vector.x +M[1][3] * vector.y + M[2][3] * vector.z + M[3][3] * vector.a
		);														
	}
	FVector TransformPosition(const FVector& vector) const
	{
		float x = M[0][0] * vector.x + M[1][0] * vector.y + M[2][0] * vector.z + M[3][0];
		float y = M[0][1] * vector.x + M[1][1] * vector.y + M[2][1] * vector.z + M[3][1];
		float z = M[0][2] * vector.x + M[1][2] * vector.y + M[2][2] * vector.z + M[3][2];
		float w = M[0][3] * vector.x + M[1][3] * vector.y + M[2][3] * vector.z + M[3][3];
		return w != 0.0f ? FVector{ x / w, y / w, z / w } : FVector{ x, y, z };
	}
};
enum OBJECTS
{
	OBJ_SPHERE,
	OBJ_CUBE,
	OBJ_TRIANGLE,
	OBJ_CAMERA,
	OBJ_PLAYER,
	OBJ_END
};
enum ARROW_DIR
{
	AD_X,
	AD_Y,
	AD_Z,
	AD_END
};
enum ControlMode
{
	CM_TRANSLATION,
	CM_SCALE,
	CM_ROTATION,
	CM_END
};
struct FQuat
{
	float w, x, y, z;

	// �⺻ ������
	FQuat() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

	// FQuat ������ �߰�: ȸ�� ��� ������ �޾Ƽ� FQuat ����
	FQuat(const FVector& Axis, float Angle)
	{
		float HalfAngle = Angle * 0.5f;
		float SinHalfAngle = sinf(HalfAngle);
		float CosHalfAngle = cosf(HalfAngle);

		x = Axis.x * SinHalfAngle;
		y = Axis.y * SinHalfAngle;
		z = Axis.z * SinHalfAngle;
		w = CosHalfAngle;
	}

	// w, x, y, z ������ �ʱ�ȭ
	FQuat(float InW, float InX, float InY, float InZ) : w(InW), x(InX), y(InY), z(InZ) {}

	// ���ʹϾ��� ���� ���� (ȸ�� ����)
	FQuat operator*(const FQuat& Other) const
	{
		return FQuat(
			w * Other.w - x * Other.x - y * Other.y - z * Other.z,
			w * Other.x + x * Other.w + y * Other.z - z * Other.y,
			w * Other.y - x * Other.z + y * Other.w + z * Other.x,
			w * Other.z + x * Other.y - y * Other.x + z * Other.w
		);
	}

	// ���� ȸ��
	FVector RotateVector(const FVector& Vec) const
	{
		// ���͸� ���ʹϾ����� ��ȯ
		FQuat vecQuat(0.0f, Vec.x, Vec.y, Vec.z);
		// ȸ�� ���� (q * vec * q^-1)
		FQuat conjugate = FQuat(w, -x, -y, -z);  // ���ʹϾ��� �ӷ�
		FQuat result = *this * vecQuat * conjugate;

		return FVector(result.x, result.y, result.z); // ȸ���� ���� ��ȯ
	}

	// ���� ���ʹϾ� ���� Ȯ��
	bool IsNormalized() const
	{
		return fabs(w * w + x * x + y * y + z * z - 1.0f) < 1e-6f;
	}

	// ���ʹϾ� ����ȭ (���� ���ʹϾ����� ����)
	FQuat Normalize() const
	{
		float magnitude = sqrtf(w * w + x * x + y * y + z * z);
		return FQuat(w / magnitude, x / magnitude, y / magnitude, z / magnitude);
	}

	// ȸ�� ������ �����κ��� ���ʹϾ� ���� (axis-angle ���)
	static FQuat FromAxisAngle(const FVector& Axis, float Angle)
	{
		float halfAngle = Angle * 0.5f;
		float sinHalfAngle = sinf(halfAngle);
		return FQuat(cosf(halfAngle), Axis.x * sinHalfAngle, Axis.y * sinHalfAngle, Axis.z * sinHalfAngle);
	}

	// ���ʹϾ��� ȸ�� ��ķ� ��ȯ
	FMatrix ToMatrix() const
	{
		// ȸ�� ����� ��� ���
		FMatrix RotationMatrix;
		RotationMatrix.M[0][0] = 1.0f - 2.0f * (y * y + z * z);
		RotationMatrix.M[0][1] = 2.0f * (x * y - w * z);
		RotationMatrix.M[0][2] = 2.0f * (x * z + w * y);

		RotationMatrix.M[1][0] = 2.0f * (x * y + w * z);
		RotationMatrix.M[1][1] = 1.0f - 2.0f * (x * x + z * z);
		RotationMatrix.M[1][2] = 2.0f * (y * z - w * x);

		RotationMatrix.M[2][0] = 2.0f * (x * z - w * y);
		RotationMatrix.M[2][1] = 2.0f * (y * z + w * x);
		RotationMatrix.M[2][2] = 1.0f - 2.0f * (x * x + y * y);

		RotationMatrix.M[3][0] = RotationMatrix.M[3][1] = RotationMatrix.M[3][2] = 0.0f;
		RotationMatrix.M[3][3] = 1.0f;

		return RotationMatrix;
	}
};
