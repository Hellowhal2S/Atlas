#include "CubeComp.h"
#include "JungleMath.h"
#include "World.h"
#include "ShowFlags.h"
#include "PrimitiveBatch.h"
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
	FMatrix Model = JungleMath::CreateModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());

	// ���� MVP ���
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	FEngineLoop::renderer.UpdateNormalConstantBuffer(Model);
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);

	if (ShowFlags::GetInstance().currentFlags & static_cast<uint64>(EEngineShowFlags::SF_AABB))
		UPrimitiveBatch::GetInstance().AddBoxForCube(GetWorldLocation(), { 1,1,1,1 }, Model);
	if (ShowFlags::GetInstance().currentFlags & static_cast<uint64>(EEngineShowFlags::SF_Primitives))
		Super::Render();
}
