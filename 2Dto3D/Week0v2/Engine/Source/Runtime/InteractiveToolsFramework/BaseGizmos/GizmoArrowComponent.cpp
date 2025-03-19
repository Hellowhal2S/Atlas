#include "GizmoArrowComponent.h"
#include "Engine/Source/Runtime/Core/Math/JungleMath.h"
#include "Components/Player.h"
UGizmoArrowComponent::UGizmoArrowComponent()
{
}

UGizmoArrowComponent::~UGizmoArrowComponent()
{
}

void UGizmoArrowComponent::Initialize()
{
	Super::Initialize();
}

void UGizmoArrowComponent::Update(double deltaTime)
{
	Super::Update(deltaTime);

}

void UGizmoArrowComponent::Release()
{
}

void UGizmoArrowComponent::Render()
{
#pragma region GizmoDepth
	ID3D11DepthStencilState* DepthStateDisable = FEngineLoop::graphicDevice.DepthStateDisable;
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(DepthStateDisable, 0);
#pragma endregion GizmoDepth

	if (!GetWorld()->GetPickingObj() || GetWorld()->GetPlayer()->GetControlMode() != CM_TRANSLATION)
		return;
	FMatrix Model = JungleMath::CreateModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());

	// ���� MVP ���
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	
	
	if (this == GetWorld()->GetPickingGizmo()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	FEngineLoop::graphicDevice.DeviceContext->RSSetState(FEngineLoop::graphicDevice.RasterizerStateSOLID); // fill solid�� ������.
	Super::Render();
	FEngineLoop::graphicDevice.DeviceContext->RSSetState(FEngineLoop::graphicDevice.GetCurrentRasterizer()); // ���� �����Ͷ����� �缳��.

#pragma region GizmoDepth
	ID3D11DepthStencilState* originalDepthState = FEngineLoop::graphicDevice.DepthStencilState;
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(originalDepthState, 0);
#pragma endregion GizmoDepth
}
