#include "JungleMath.h"
#include <DirectXMath.h>

using namespace DirectX;
FVector4 JungleMath::ConvertV3ToV4(FVector vec3)
{
	FVector4 newVec4;
	newVec4.x = vec3.x;
	newVec4.y = vec3.y;
	newVec4.z = vec3.z;
	return newVec4;
}



FMatrix JungleMath::CreateModelMatrix(FVector translation, FVector rotation, FVector scale)
{
    FVector newTranslation = FVector(translation.x, translation.z, translation.y * -1);
    FVector newScale = FVector(scale.x, scale.z, scale.y * -1);
    FVector newRotation = FVector(rotation.x, rotation.z, rotation.y * -1);
    // 1. 스케일 행렬
    XMMATRIX scaleMatrix = XMMatrixScaling(newScale.x, newScale.y, newScale.z);

    // 2. 회전 행렬 (쿼터니언 사용)
    XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), DegToRad(newRotation.x));
    XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), DegToRad(newRotation.y));
    XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), DegToRad(newRotation.z));

    XMVECTOR rotationQuat = XMQuaternionMultiply(quatZ, XMQuaternionMultiply(quatY, quatX));
    rotationQuat = XMQuaternionNormalize(rotationQuat);  // 정규화 필수

    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);

    // 3. 이동 행렬
    XMMATRIX translationMatrix = XMMatrixTranslation(newTranslation.x, newTranslation.y, newTranslation.z);

    // 최종 변환 행렬 (회전 -> 스케일 -> 이동 순서)
    XMMATRIX finalMatrix = XMMatrixMultiply(XMMatrixMultiply(scaleMatrix,rotationMatrix), translationMatrix);

    // XMMATRIX -> FMatrix 변환
    FMatrix result = FMatrix::Identity;  // 기본값 설정 (단위 행렬)

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.M[i][j] = finalMatrix.r[i].m128_f32[j];  // XMMATRIX에서 FMatrix로 값 복사
        }
    }
    return result;
}

//FMatrix JungleMath::CreateModelMatrix(FVector translation, FVector rotation, FVector scale)
//{
//    // Unreal 좌표계 변환: (X, Y, Z) → (Y, Z, X)
//    FVector unrealTranslation(translation.y, translation.z, translation.x);
//    FVector unrealRotation(rotation.y, rotation.z, rotation.x);
//    FVector unrealScale(scale.y, scale.z, scale.x);
//
//    // 1. 스케일 행렬
//    XMMATRIX scaleMatrix = XMMatrixScaling(unrealScale.x, unrealScale.y, unrealScale.z);
//
//    // 2. 회전 행렬 (Yaw-Pitch-Roll, 쿼터니언 사용)
//    XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), DegToRad(unrealRotation.x));
//    XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), DegToRad(unrealRotation.y));
//    XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), DegToRad(unrealRotation.z));
//
//    XMVECTOR rotationQuat = XMQuaternionMultiply(quatZ, XMQuaternionMultiply(quatY, quatX));
//    rotationQuat = XMQuaternionNormalize(rotationQuat);  // 정규화 필수
//
//    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);
//
//    // 3. 이동 행렬
//    XMMATRIX translationMatrix = XMMatrixTranslation(unrealTranslation.x, unrealTranslation.y, unrealTranslation.z);
//
//    // 최종 변환 행렬 (회전 -> 스케일 -> 이동 순서)
//    XMMATRIX finalMatrix = XMMatrixMultiply(XMMatrixMultiply(scaleMatrix, rotationMatrix), translationMatrix);
//
//    // XMMATRIX -> FMatrix 변환
//    FMatrix result = FMatrix::Identity;
//
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            result.M[i][j] = finalMatrix.r[i].m128_f32[j];  // XMMATRIX에서 FMatrix로 값 복사
//        }
//    }
//
//    return result;  // 이미 Unreal 좌표계이므로 추가 변환 불필요
//}

FMatrix JungleMath::CreateViewMatrix(FVector eye, FVector target, FVector up)
{
    FVector zAxis = (target - eye).Normalize();  // DirectX는 LH이므로 -z가 아니라 +z 사용
    FVector xAxis = (up.Cross(zAxis)).Normalize();
    FVector yAxis = zAxis.Cross(xAxis);

    FMatrix View;
    View.M[0][0] = xAxis.x; View.M[0][1] = yAxis.x; View.M[0][2] = zAxis.x; View.M[0][3] = 0;
    View.M[1][0] = xAxis.y; View.M[1][1] = yAxis.y; View.M[1][2] = zAxis.y; View.M[1][3] = 0;
    View.M[2][0] = xAxis.z; View.M[2][1] = yAxis.z; View.M[2][2] = zAxis.z; View.M[2][3] = 0;
    View.M[3][0] = -xAxis.Dot(eye);
    View.M[3][1] = -yAxis.Dot(eye);
    View.M[3][2] = -zAxis.Dot(eye);
    View.M[3][3] = 1;

    return View;
}

FMatrix JungleMath::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
    float tanHalfFOV = tan(fov / 2.0f);
    float depth = farPlane - nearPlane;

    FMatrix Projection = {};
    Projection.M[0][0] = 1.0f / (aspect * tanHalfFOV);
    Projection.M[1][1] = 1.0f / tanHalfFOV;
    Projection.M[2][2] = farPlane / depth;
    Projection.M[2][3] = 1.0f;
    Projection.M[3][2] = -(nearPlane * farPlane) / depth;
    Projection.M[3][3] = 0.0f;  

    return Projection;
}
//
//FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
//{
//    // 회전 값 (degree -> radian 변환)
//    float pitch = XMConvertToRadians(rotation.x); // X축 (Pitch)
//    float yaw = XMConvertToRadians(rotation.y); // Y축 (Yaw)
//    float roll = XMConvertToRadians(rotation.z); // Z축 (Roll)
//
//    // Yaw-Pitch-Roll 회전 행렬 생성
//    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//    // 원본 벡터를 XMVECTOR로 변환
//    XMVECTOR vec = XMVectorSet(origin.x, origin.y, origin.z, 1.0f);
//
//    // 회전 적용
//    vec = XMVector3Transform(vec, rotMatrix);
//
//    // 결과를 FVector로 변환하여 반환
//    FVector rotated;
//    rotated.x = XMVectorGetX(vec);
//    rotated.y = XMVectorGetY(vec);
//    rotated.z = XMVectorGetZ(vec);
//
//    return rotated;
//}

FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
{
    // 회전 각도를 라디안으로 변환
    float pitch = DegToRad(rotation.x) * 0.5f;
    float yaw = DegToRad(rotation.y) * 0.5f;
    float roll = DegToRad(rotation.z) * 0.5f;

    // 쿼터니언 생성 (Yaw-Pitch-Roll 순서)
    float cosX = cosf(pitch), sinX = sinf(pitch);
    float cosY = cosf(yaw), sinY = sinf(yaw);
    float cosZ = cosf(roll), sinZ = sinf(roll);

    FQuat quaternion;
    quaternion.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    quaternion.x = sinX * cosY * cosZ - cosX * sinY * sinZ;
    quaternion.y = cosX * sinY * cosZ + sinX * cosY * sinZ;
    quaternion.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    // 쿼터니언을 이용해 벡터 회전 적용
    return quaternion.RotateVector(origin);
}
//FVector JungleMath::FVectorRotate(FVector& origin, const FVector& rotation)
//{
//    // DirectX → Unreal 좌표계 변환 (X, Y, Z) → (Y, Z, X)
//    FVector unrealOrigin(origin.y, origin.z, origin.x);
//    FVector unrealRotation(rotation.y, rotation.z, rotation.x);
//
//    // 회전 각도를 라디안으로 변환
//    float pitch = DegToRad(unrealRotation.x) * 0.5f;
//    float yaw = DegToRad(unrealRotation.y) * 0.5f;
//    float roll = DegToRad(unrealRotation.z) * 0.5f;
//
//    // 쿼터니언 생성 (Yaw-Pitch-Roll 순서)
//    float cosX = cosf(pitch), sinX = sinf(pitch);
//    float cosY = cosf(yaw), sinY = sinf(yaw);
//    float cosZ = cosf(roll), sinZ = sinf(roll);
//
//    FQuat quaternion;
//    quaternion.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
//    quaternion.x = sinX * cosY * cosZ - cosX * sinY * sinZ;
//    quaternion.y = cosX * sinY * cosZ + sinX * cosY * sinZ;
//    quaternion.z = cosX * cosY * sinZ - sinX * sinY * cosZ;
//
//    // 쿼터니언을 이용해 벡터 회전 적용
//    FVector rotatedUnreal = quaternion.RotateVector(unrealOrigin);
//
//    // Unreal → DirectX 좌표계 변환 (Y, Z, X) → (X, Y, Z)
//    return FVector(rotatedUnreal.z, rotatedUnreal.x, rotatedUnreal.y);
//}
FMatrix JungleMath::ConvertToUnrealMatrix(const FMatrix& directXMatrix)
{
    // DirectX -> Unreal 좌표계 변환 행렬
    FMatrix conversionMatrix;
    conversionMatrix.M[0][0] = 0;  conversionMatrix.M[0][1] = 0;  conversionMatrix.M[0][2] = 1;  conversionMatrix.M[0][3] = 0;
    conversionMatrix.M[1][0] = 1;  conversionMatrix.M[1][1] = 0;  conversionMatrix.M[1][2] = 0;  conversionMatrix.M[1][3] = 0;
    conversionMatrix.M[2][0] = 0;  conversionMatrix.M[2][1] = 1;  conversionMatrix.M[2][2] = 0;  conversionMatrix.M[2][3] = 0;
    conversionMatrix.M[3][0] = 0;  conversionMatrix.M[3][1] = 0;  conversionMatrix.M[3][2] = 0;  conversionMatrix.M[3][3] = 1;

    // 변환 적용 (변환 행렬 * 원본 행렬)
    return conversionMatrix * directXMatrix;
}


float JungleMath::RadToDeg(float radian)
{
    return radian * (180.0f / PI);
}

float JungleMath::DegToRad(float degree)
{
    return degree * (PI / 180.0f);
}


