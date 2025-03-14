#include "SphereComp.h"
#include "JungleMath.h"
#include "World.h"
USphereComp::USphereComp() : UPrimitiveComponent("Sphere")
{
}

USphereComp::~USphereComp()
{
}

void USphereComp::Initialize()
{
	Super::Initialize();
}

void USphereComp::Update(double deltaTime)
{
}

void USphereComp::Release()
{
}

void USphereComp::Render()
{
	FMatrix Model = JungleMath::CreateModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());

	// ���� MVP ���
	FMatrix MVP = Model * GetEngine().View * GetEngine().Projection;
	if (this == GetWorld()->GetPickingObj()) {
		FEngineLoop::renderer.UpdateConstant(MVP, 1.0f);
	}
	else
		FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);
	UPrimitiveBatch::GetInstance().AddBox(GetWorldLocation(), { 1,1,1,1 }, Model);
	UPrimitiveBatch::GetInstance().AddCone(GetWorldLocation(), 10, 5, 100, { 1,1,1,1 }, Model);
	Super::Render();
}
