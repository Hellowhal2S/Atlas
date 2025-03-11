#include "Player.h"
#include <Windows.h>
#include "GraphicDevice.h"
#include "World.h"
#include "Define.h"
#include "EngineLoop.h"
#include "PrimitiveComponent.h"
#include "JungleMath.h"
#include <DirectXMath.h>
#include "ArrowComp.h"
using namespace DirectX;

UPlayer::UPlayer()
{
}

UPlayer::~UPlayer()
{
}

void UPlayer::Initialize()
{
	Super::Initialize();
}

void UPlayer::Update(double deltaTime)
{
	Input();
}

void UPlayer::Release()
{
}

void UPlayer::Input()
{

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 )
	{
		if (!bLeftMouseDown) {
			bLeftMouseDown = true;

			POINT mousePos;
			GetCursorPos(&mousePos);
			GetCursorPos(&m_LastMousePos);


			ScreenToClient(GEngineLoop.hWnd, &mousePos);

			FVector rayOrigin;
			FVector rayDir;

			ScreenToRay(mousePos.x, mousePos.y, GEngineLoop.View, GEngineLoop.Projection, rayOrigin, rayDir);
			PickObj(rayOrigin, rayDir);
			PickGzimo(rayOrigin, rayDir);
		}
		else
		{
			PickedObjControl();
		}
	}
	else
	{
		if (bLeftMouseDown) {
				bLeftMouseDown = false; // ���콺 ������ ��ư�� ���� ���� �ʱ�ȭ
				GetWorld()->SetPickingGizmo(nullptr);
		}
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!bSpaceDown) {
			AddMode();
			bSpaceDown = true;
		}
	}
	else
	{
		if (bSpaceDown)
		{
			bSpaceDown = false;
		}
	}
}

void UPlayer::PickGzimo(FVector& rayOrigin, FVector& rayDir)
{
	if (GetWorld()->GetPickingObj()) {
		for (int i = 0;i < 3;++i)
		{
			UArrowComp* Arrow = static_cast<UArrowComp*>(GetWorld()->LocalGizmo[i]);
			float Scale = 0.0f;
			FVector DetectLoc;
			if (i == 0) {
				Scale = GetWorld()->LocalGizmo[0]->GetScale().x;
				DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetRightVector();
			}
			else if (i == 1) {
				Scale = GetWorld()->LocalGizmo[1]->GetScale().y;
				DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetUpVector();
			}
			else if (i == 2) {
				Scale = GetWorld()->LocalGizmo[2]->GetScale().z;
				DetectLoc = Arrow->GetLocation() + GetWorld()->GetPickingObj()->GetForwardVector();
			}
			float minDistance = 1000000.0f;
			float Distance = 0.0f;
			if (RayIntersectsObject(rayOrigin, rayDir, GetWorld()->LocalGizmo[i], Distance))
			{
				if (minDistance > Distance)
				{
					GetWorld()->SetPickingGizmo(GetWorld()->LocalGizmo[i]);
					minDistance = Distance;
				}
			}
		}
	}
}

void UPlayer::PickObj(FVector& rayOrigin, FVector& rayDir)
{
	UObject* Possible = nullptr;

	for (auto iter : GetWorld()->GetObjectArr())
	{
		UPrimitiveComponent* pObj = nullptr;
		if (iter->IsA(UPrimitiveComponent::StaticClass())) {
			 pObj = static_cast<UPrimitiveComponent*>(iter);
		}
		if (pObj && pObj->GetType() != "Arrow")
		{
			float minDistance = 10000000.0f;
			float Distance = 0.0f;
			if (RayIntersectsObject(rayOrigin, rayDir, pObj, Distance))
			{

				if (minDistance > Distance) {
					Possible = pObj;
					minDistance = Distance;
				}
			}
		}
	}
	if (Possible)
		GetWorld()->SetPickingObj(Possible);
}

void UPlayer::AddMode()
{
	cMode = static_cast<ControlMode>(((cMode + 1) % ControlMode::CM_END));
}

void UPlayer::ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix, FVector& rayOrigin, FVector& rayDir)
{
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	FEngineLoop::graphicDevice.DeviceContext->RSGetViewports(&numViewports, &viewport);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;
	float x = (2.0f * screenX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * screenY) / screenHeight;

	// �������� ����� ���
	FMatrix inverseProj = FMatrix::Inverse(projectionMatrix);

	// NDC���� �� �������� ��ȯ
	FVector4 nearPoint = inverseProj.TransformFVector4(FVector4(x, y, 0.0f, 1.0f));
	FVector4 farPoint = inverseProj.TransformFVector4(FVector4(x, y, 1.0f, 1.0f));

	// W�� 1�� ����ȭ
	nearPoint = nearPoint / nearPoint.a;
	farPoint = farPoint / farPoint.a;

	// �� ����� �ݿ��Ͽ� ���� �������� ��ȯ
	FMatrix inverseView = FMatrix::Inverse(viewMatrix);
	FVector nearWorld = inverseView.TransformPosition(FVector(nearPoint.x, nearPoint.y, nearPoint.z));
	FVector farWorld = inverseView.TransformPosition(FVector(farPoint.x, farPoint.y, farPoint.z));

	// ������ �������� ���� ���
	rayOrigin = nearWorld;
	rayDir = farWorld - nearWorld;
	rayDir = rayDir.Normalize();

	char message[256];
	sprintf_s(message, "Ray Origin: (%.2f, %.2f, %.2f)\nRay Direction: (%.2f, %.2f, %.2f)",
		rayOrigin.x, rayOrigin.y, rayOrigin.z, rayDir.x, rayDir.y, rayDir.z);
	//MessageBoxA(nullptr, message, "ScreenToRay Output", MB_OK);
}

bool UPlayer::RayIntersectsSphere(const FVector& rayOrigin, const FVector& rayDir, const FVector& sphereCenter, float sphereRadius)
{
	// ���̿� ��ü�� ���� ���� ���
	FVector oc;
	oc.x = rayOrigin.x - sphereCenter.x;
	oc.y = rayOrigin.y - sphereCenter.y;
	oc.z = rayOrigin.z - sphereCenter.z;

	float b = 2.0f * (rayDir.x * oc.x + rayDir.y * oc.y + rayDir.z * oc.z);
	float c = (oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4.0f * c;
	return discriminant > 0;
}

//OBB�� ����
bool UPlayer::RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, UObject* obj, float& hitDistance)
{
	// ������Ʈ�� ���� ��ȯ ��� ���� (��ġ, ȸ�� ����)
	FMatrix rotationMatrix = FMatrix::CreateRotation(
		obj->GetRotation().x,
		obj->GetRotation().y,
		obj->GetRotation().z
	);

	FMatrix translationMatrix = FMatrix::CreateTranslationMatrix(obj->GetLocation());

	// ���� ��ȯ ���
	FMatrix worldMatrix = rotationMatrix * translationMatrix;

	// OBB�� ���� X, Y, Z �� ��������
	FVector axisX(worldMatrix.M[0][0], worldMatrix.M[0][1], worldMatrix.M[0][2]);
	FVector axisY(worldMatrix.M[1][0], worldMatrix.M[1][1], worldMatrix.M[1][2]);
	FVector axisZ(worldMatrix.M[2][0], worldMatrix.M[2][1], worldMatrix.M[2][2]);

	// ��������� Ȯ��
	//bool isGizmo = (dynamic_cast<UGizmoComp*>(obj) != nullptr);

	// OBB�� �� ũ�� ���� (������� ��� Y,Z �������� 0.2�� ����)
	FVector halfSize = obj->GetScale();
	//if (isGizmo)
	//{
	//	halfSize.y = 0.2f;
	//	halfSize.z = 0.2f;
	//}

	FVector p = obj->GetLocation() - rayOrigin; // ���� �������� OBB �߽ɱ����� ����
	FVector d = rayDir.Normalize(); // ���� ���� ����ȭ

	float tMin = -FLT_MAX, tMax = FLT_MAX;

	// �� ��(X, Y, Z)�� ���� ���̿��� �浹 �˻� ����
	FVector axes[3] = { axisX, axisY, axisZ };
	float halfSizes[3] = { halfSize.x, halfSize.y, halfSize.z };

	for (int i = 0; i < 3; i++)
	{
		float e = axes[i].Dot(p);  // �������� Ÿ�ٱ����� �Ÿ��� �ش� ������ ����
		float f = axes[i].Dot(d);  // ���̸� ����

		if (fabs(f) > 1e-6)  // ���� ���Ͱ� 0�̸� ū�ϳ�
		{
			float t1 = (e - halfSizes[i]) / f;
			float t2 = (e + halfSizes[i]) / f;

			if (t1 > t2) std::swap(t1, t2); // t1�� �׻� ���� ���� �ǵ��� ����

			tMin = max(tMin, t1);  // ������ ��� �ٿ�����
			tMax = min(tMax, t2);

			if (tMin > tMax) return false; // �浹 ����
		}
		else if (-e - halfSizes[i] > 0 || -e + halfSizes[i] < 0)
		{
			return false; // ���̰� OBB ���ο� ����, ���������� ����
		}
	}

	hitDistance = tMin;
	return true;
}

void UPlayer::PickedObjControl()
{
	// ���콺 �̵��� ���
	if (GetWorld()->GetPickingObj() && GetWorld()->GetPickingGizmo()) {
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		// ���콺 �̵� ���� ���
		int32 deltaX = currentMousePos.x - m_LastMousePos.x;
		int32 deltaY = currentMousePos.y - m_LastMousePos.y;

		UObject* pObj = GetWorld()->GetPickingObj();
		UArrowComp* Arrow = static_cast<UArrowComp*>(GetWorld()->GetPickingGizmo());
		switch (cMode)
		{
		case CM_TRANSLATION:
			ControlTranslation(pObj, Arrow, deltaX, deltaY);
			break;
		case CM_SCALE:
			ControlScale(pObj, Arrow, deltaX, deltaY);

			break;
		case CM_ROTATION:
			ControlRoation(Arrow, pObj, deltaX, deltaY);
			break;
		}
		// ���ο� ���콺 ��ġ ����
		m_LastMousePos = currentMousePos;
	}
}

void UPlayer::ControlRoation(UArrowComp* Arrow, UObject* pObj, int32 deltaX, int32 deltaY)
{
	switch (Arrow->GetDir())
	{
	case AD_X:
		if (GetWorld()->GetCamera()->GetForwardVector().z >= 0) {
			pObj->AddRotation(FVector(1.0f, 0.0f, 0.0f) * deltaX * 0.1f);
			pObj->AddRotation(FVector(1.0f, 0.0f, 0.0f) * deltaY * 0.1f);
		}
		else {
			pObj->AddRotation(FVector(1.0f, 0.0f, 0.0f) * deltaX * -0.1f);
			pObj->AddRotation(FVector(1.0f, 0.0f, 0.0f) * deltaY * -0.1f);
		}

		break;
	case AD_Y:
		if (pObj->GetUpVector().y >= 0)
		{
			pObj->AddRotation(FVector(0.0f, 1.0f, 0.0f) * deltaX * -0.1f);
			pObj->AddRotation(FVector(0.0f, 1.0f, 0.0f) * deltaX * -0.1f);
		}
		else {
			pObj->AddRotation(FVector(0.0f, 1.0f, 0.0f) * deltaY * 0.1f);
			pObj->AddRotation(FVector(0.0f, 1.0f, 0.0f) * deltaX * 0.1f);
		}
		break;
	case AD_Z:

		if (GetWorld()->GetCamera()->GetForwardVector().x <= 0) {
			pObj->AddRotation(FVector(0.0f, 0.0f, 1.0f) * deltaX * 0.1f);
			pObj->AddRotation(FVector(0.0f, 0.0f, 1.0f) * deltaY * 0.1f);
		}
		else {
			pObj->AddRotation(FVector(0.0f, 0.0f, 1.0f) * deltaX * -0.1f);
			pObj->AddRotation(FVector(0.0f, 0.0f, 1.0f) * deltaX * -0.1f);
		}
		break;
	default:
		break;
	}
}

void UPlayer::ControlTranslation(UObject* pObj, UArrowComp* Arrow, int32 deltaX, int32 deltaY)
{
	float xdir = pObj->GetRightVector().x >= 0 ? 1.0 : -1.0;
	float zdir = pObj->GetForwardVector().z >= 0 ? 1.0 : -1.0;
	switch (Arrow->GetDir())
	{
	case AD_X:
		if (GetWorld()->GetCamera()->GetForwardVector().z >= 0)
			pObj->AddLocation(pObj->GetRightVector() * deltaX * 0.01f * xdir);
		else
			pObj->AddLocation(pObj->GetRightVector() * deltaX * -0.01f * xdir);
		break;
	case AD_Y:
		if (pObj->GetUpVector().y >= 0)
			pObj->AddLocation((pObj->GetUpVector() * deltaY * 0.01f) * -1);
		else
			pObj->AddLocation((pObj->GetUpVector() * deltaY * 0.01f));
		break;
	case AD_Z:

		if (GetWorld()->GetCamera()->GetForwardVector().x <= 0)
			pObj->AddLocation(pObj->GetForwardVector() * deltaX * 0.01f * zdir);
		else
			pObj->AddLocation(pObj->GetForwardVector() * deltaX * -0.01f * zdir);
		break;
	default:
		break;
	}
}

void UPlayer::ControlScale(UObject* pObj, UArrowComp* Arrow, int32 deltaX, int32 deltaY)
{
	float xdir = pObj->GetRightVector().x >= 0 ? 1.0 : -1.0;
	float zdir = pObj->GetForwardVector().z >= 0 ? 1.0 : -1.0;
	switch (Arrow->GetDir())
	{
	case AD_X:
		if (GetWorld()->GetCamera()->GetForwardVector().z >= 0)
			pObj->AddScale(FVector(1.0f, 0.0f, 0.0f) * deltaX * 0.01f * xdir);
		else
			pObj->AddScale(FVector(1.0f, 0.0f, 0.0f) * deltaX * -0.01f * xdir);
		break;
	case AD_Y:
		if (pObj->GetUpVector().y >= 0)
			pObj->AddScale(FVector(0.0f, 1.0f, 0.0f) * deltaY * -0.01f);
		else
			pObj->AddScale(FVector(0.0f, 1.0f, 0.0f) * deltaY * 0.01f);
		break;
	case AD_Z:

		if (GetWorld()->GetCamera()->GetForwardVector().x <= 0)
			pObj->AddScale(FVector(0.0f, 0.0f, 1.0f) * deltaX * 0.01f * zdir);
		else
			pObj->AddScale(FVector(0.0f, 0.0f, 1.0f) * deltaX * -0.01f * zdir);
		break;
	default:
		break;
	}
}
