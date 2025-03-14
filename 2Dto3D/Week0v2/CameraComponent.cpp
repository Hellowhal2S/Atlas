#include "CameraComponent.h"
#include "JungleMath.h"
#include "World.h"
UCameraComponent::UCameraComponent()
{
}

UCameraComponent::~UCameraComponent()
{
}

void UCameraComponent::Initialize()
{
	Super::Initialize();
	RelativeLocation = FVector(0.0f, 0.0f, 0.5f);
	fov = 60.f;
}

void UCameraComponent::Update(double deltaTime)
{
	Input();
	zAxis = (RelativeLocation + GetForwardVector() - RelativeLocation).Normalize();
	FVector X = FVector(0.0f, 0.0f, 1.0f);
	xAxis = (JungleMath::FVectorRotate(X,RelativeRotation).Cross(zAxis)).Normalize();
	yAxis = zAxis.Cross(xAxis);
}

void UCameraComponent::Release()
{
}

void UCameraComponent::Input()
{
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) // VK_RBUTTON은 마우스 오른쪽 버튼을 나타냄
	{
		if (!bRightMouseDown)
		{
			// 마우스 오른쪽 버튼을 처음 눌렀을 때, 마우스 위치 초기화
			bRightMouseDown = true;
			GetCursorPos(&m_LastMousePos);
		}
		else
		{
			// 마우스 이동량 계산
			POINT currentMousePos;
			GetCursorPos(&currentMousePos);

			// 마우스 이동 차이 계산
			int32 deltaX = currentMousePos.x - m_LastMousePos.x;
			int32 deltaY = currentMousePos.y - m_LastMousePos.y;

			// Yaw(좌우 회전) 및 Pitch(상하 회전) 값 변경
			RotateYaw(deltaX * 0.1f);  // X 이동에 따라 좌우 회전
			RotatePitch(deltaY * 0.1f);  // Y 이동에 따라 상하 회전

			// 새로운 마우스 위치 저장
			m_LastMousePos = currentMousePos;
		}
	}
	else
	{
		bRightMouseDown = false; // 마우스 오른쪽 버튼을 떼면 상태 초기화
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		MoveRight(-1.f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		MoveRight(1.f);
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		MoveForward(1.f);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		MoveForward(-1.f);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		MoveUp(1.f);
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		MoveUp(-1.f);
	}
	if (GetAsyncKeyState('J') & 0x8000)
	{
		RotateYaw(-1.f);
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		RotateYaw(1.f);
	}
	if (GetAsyncKeyState('I') & 0x8000)
	{
		RotatePitch(-1.f);
	}
	if (GetAsyncKeyState('K') & 0x8000)
	{
		RotatePitch(1.f);
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		if (!GetWorld()->GetPickingObj()) return;
		GetWorld()->GetPickingObj()->SetLocation(GetWorld()->GetPickingObj()->GetWorldLocation() + GetWorld()->GetPickingObj()->GetRightVector());
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		if (!GetWorld()->GetPickingObj()) return;
		GetWorld()->GetPickingObj()->SetLocation(GetWorld()->GetPickingObj()->GetWorldLocation() + GetWorld()->GetPickingObj()->GetForwardVector());
	}
}

void UCameraComponent::MoveForward(float _Value)
{
	RelativeLocation = RelativeLocation + GetForwardVector() * m_mouseSpeed * _Value;
}

void UCameraComponent::MoveRight(float _Value)
{
	RelativeLocation = RelativeLocation + GetRightVector() * m_mouseSpeed * _Value;
}

void UCameraComponent::MoveUp(float _Value)
{
	RelativeLocation.z += _Value *0.5f;
}

void UCameraComponent::RotateYaw(float _Value)
{
	RelativeRotation.z += _Value;
}

void UCameraComponent::RotatePitch(float _Value)
{
	RelativeRotation.y += _Value;
	if (RelativeRotation.y < -90.0f)
		RelativeRotation.y = -90.0f;
	if (RelativeRotation.y > 90.0f)
		RelativeRotation.y = 90.0f;

}



