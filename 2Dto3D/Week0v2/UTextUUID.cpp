#include "UTextUUID.h"

UTextUUID::UTextUUID()
{
	SetScale(FVector(0.1f, 0.25f, 0.25f));
}

UTextUUID::~UTextUUID()
{
}

void UTextUUID::Render()
{
	//�÷��� ó��
#pragma region GizmoDepth
	ID3D11DepthStencilState* DepthStateDisable = FEngineLoop::graphicDevice.DepthStateDisable;
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(DepthStateDisable, 0);
#pragma endregion GizmoDepth

	TextMVPRendering();

#pragma region GizmoDepth
	ID3D11DepthStencilState* originalDepthState = FEngineLoop::graphicDevice.DepthStencilState;
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(originalDepthState, 0);
#pragma endregion GizmoDepth
}

int UTextUUID::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
	return 0;
}

void UTextUUID::SetUUID(uint32 UUID)
{
	SetText(std::to_wstring(UUID));
}


