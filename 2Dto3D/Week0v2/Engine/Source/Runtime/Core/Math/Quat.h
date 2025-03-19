#pragma once

// ���ʹϾ�
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

	// (���ʹϾ�) ���� ȸ��
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

	static FQuat CreateRotation(float roll, float pitch, float yaw)
	{
		// ������ �������� ��ȯ
		float radRoll = roll * (3.14159265359f / 180.0f);
		float radPitch = pitch * (3.14159265359f / 180.0f);
		float radYaw = yaw * (3.14159265359f / 180.0f);

		// �� �࿡ ���� ȸ�� ���ʹϾ� ���
		FQuat qRoll = FQuat(FVector(1.0f, 0.0f, 0.0f), radRoll);  // X�� ȸ��
		FQuat qPitch = FQuat(FVector(0.0f, 1.0f, 0.0f), radPitch);  // Y�� ȸ��
		FQuat qYaw = FQuat(FVector(0.0f, 0.0f, 1.0f), radYaw);  // Z�� ȸ��

		// ȸ�� ������� ���ʹϾ� ���� (Y -> X -> Z)
		return qRoll * qPitch * qYaw;
	}
	// ���ʹϾ��� ȸ�� ��ķ� ��ȯ
	FMatrix ToMatrix() const
	{
		FMatrix RotationMatrix;
		RotationMatrix.M[0][0] = 1.0f - 2.0f * (y * y + z * z);
		RotationMatrix.M[0][1] = 2.0f * (x * y - w * z);
		RotationMatrix.M[0][2] = 2.0f * (x * z + w * y);
		RotationMatrix.M[0][3] = 0.0f;


		RotationMatrix.M[1][0] = 2.0f * (x * y + w * z);
		RotationMatrix.M[1][1] = 1.0f - 2.0f * (x * x + z * z);
		RotationMatrix.M[1][2] = 2.0f * (y * z - w * x);
		RotationMatrix.M[1][3] = 0.0f;

		RotationMatrix.M[2][0] = 2.0f * (x * z - w * y);
		RotationMatrix.M[2][1] = 2.0f * (y * z + w * x);
		RotationMatrix.M[2][2] = 1.0f - 2.0f * (x * x + y * y);
		RotationMatrix.M[2][3] = 0.0f;

		RotationMatrix.M[3][0] = RotationMatrix.M[3][1] = RotationMatrix.M[3][2] = 0.0f;
		RotationMatrix.M[3][3] = 1.0f;

		return RotationMatrix;
	}
};