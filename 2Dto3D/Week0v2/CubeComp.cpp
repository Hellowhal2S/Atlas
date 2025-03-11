#include "CubeComp.h"
#include "JungleMath.h"
#include "World.h"
UCubeComp::UCubeComp() : UPrimitiveComponent("Cube")
{
}

UCubeComp::~UCubeComp()
{
}

void UCubeComp::Initialize()
{
	Super::Initialize();
}

void UCubeComp::Update(double deltaTime)
{
}

void UCubeComp::Release()
{
}

void UCubeComp::Render()
{
	FMatrix Model = JungleMath::CreateModelMatrix(GetLocation(), GetRotation(), GetScale());

	// ���� MVP ���
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	FEngineLoop::renderer.RenderPrimitive(FEngineLoop::resourceMgr.vertexBufferCube, FEngineLoop::resourceMgr.numVerticesCube);
}
