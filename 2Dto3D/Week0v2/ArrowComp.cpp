#include "ArrowComp.h"
#include "JungleMath.h"

UArrowComp::UArrowComp() : UPrimitiveComponent("Arrow")
{
}

UArrowComp::~UArrowComp()
{
}

void UArrowComp::Initialize()
{
	Super::Initialize();
}

void UArrowComp::Update(double deltaTime)
{
}

void UArrowComp::Release()
{
}

void UArrowComp::Render()
{
#pragma region GizmoDepth
	static ID3D11DepthStencilState* gizmoDepthState = nullptr;

	if (gizmoDepthState == nullptr)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = FALSE;  // ���� �׽�Ʈ ����
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // ���� ���ۿ� ���� ����
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;  // ���� �񱳸� �׻� ���

		HRESULT hr = FEngineLoop::graphicDevice.Device->CreateDepthStencilState(&depthStencilDesc, &gizmoDepthState);
		if (FAILED(hr))
		{
			Console::GetInstance().AddLog(LogLevel::Error, "Failed to create DepthStencilState! HRESULT: ");
			return;
		}
	}

	// ����� ������ ���� ����
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(gizmoDepthState, 0);
#pragma endregion GizmoDepth

	if (!GetWorld()->GetPickingObj())
		return;
	FMatrix Model = JungleMath::CreateModelMatrix(GetLocation(), GetRotation(), GetScale());

	// ���� MVP ���
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingGizmo()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);
	switch (Dir)
	{
	case AD_X:
		FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceMgr.vertexBufferArrowX, 
			FEngineLoop::resourceMgr.numVerticesArrowX,
			FEngineLoop::resourceMgr.indiceBufferArrowX,
			FEngineLoop::resourceMgr.numIndicesArrowX
		);
		break;
	case AD_Y:
		FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceMgr.vertexBufferArrowY, FEngineLoop::resourceMgr.numVerticesArrowY);
		break;
	case AD_Z:
		FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceMgr.vertexBufferArrowZ, FEngineLoop::resourceMgr.numVerticesArrowZ);
		break;
	case AD_END:
		UE_LOG(LogLevel::Warning, "whatthe...");
		break;
	default:
		break;
	}



#pragma region GizmoDepth
	ID3D11DepthStencilState* currentState = nullptr;
	UINT stencilRef;
	FEngineLoop::graphicDevice.DeviceContext->OMGetDepthStencilState(&currentState, &stencilRef);
	if (currentState != gizmoDepthState)
	{
		Console::GetInstance().AddLog(LogLevel::Warning, "DepthStencilState was overridden before rendering gizmo!");
	}
	ID3D11DepthStencilState* originalDepthState = FEngineLoop::graphicDevice.DepthStencilState;
	FEngineLoop::graphicDevice.DeviceContext->OMSetDepthStencilState(originalDepthState, 0);
#pragma endregion GizmoDepth
}
