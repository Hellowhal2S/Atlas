#include "TransformGizmo.h"
#include "GizmoArrowComponent.h"
#include "Define.h"
#include "ObjectFactory.h"
#include "GizmoCircleComponent.h"
#include "Player.h"
#include "GizmoRectangleComponent.h"
UTransformGizmo::UTransformGizmo()
{
	UObject* obj = FObjectFactory::ConstructObject<UGizmoArrowComponent>();
	UGizmoArrowComponent* ArrowX = static_cast<UGizmoArrowComponent*>(obj);
	ArrowX->SetType("ArrowX");
	ArrowX->SetParent(this);
	AttachChildren.push_back(ArrowX);
	//GetWorld()->GetObjectArr().push_back(ArrowX);
	ArrowArr.push_back(ArrowX);

	obj = FObjectFactory::ConstructObject<UGizmoArrowComponent>();
	UGizmoArrowComponent* ArrowY = static_cast<UGizmoArrowComponent*>(obj);
	ArrowY->SetType("ArrowY");

	ArrowY->SetParent(this);
	ArrowY->SetDir(ARROW_DIR::AD_Y);
	AttachChildren.push_back(ArrowY);
	//GetWorld()->GetObjectArr().push_back(ArrowY);
	ArrowArr.push_back(ArrowY);


	obj = FObjectFactory::ConstructObject<UGizmoArrowComponent>();
	UGizmoArrowComponent* ArrowZ = static_cast<UGizmoArrowComponent*>(obj);
	AttachChildren.push_back(ArrowZ);
	ArrowZ->SetType("ArrowZ");
	ArrowZ->SetParent(this);
	ArrowZ->SetDir(ARROW_DIR::AD_Z);
	AttachChildren.push_back(ArrowZ);
	//GetWorld()->GetObjectArr().push_back(ArrowZ);
	ArrowArr.push_back(ArrowZ);

	UGizmoCircleComponent* disc = new UGizmoCircleComponent(EPrimitiveColor::RED_X, 0.90, "DiscX");;
	disc->SetType("CircleX");
	disc->SetRotation(FVector(0.0f,0.0f,0.0f));
	disc->SetParent(this);
	AttachChildren.push_back(disc);
	//GetWorld()->GetObjectArr().push_back(disc);
	CircleArr.push_back(disc);

	disc = new UGizmoCircleComponent(EPrimitiveColor::GREEN_Y, 0.90, "DiscY");
	disc->SetType("CircleY");
	disc->SetParent(this);
	AttachChildren.push_back(disc);
	//GetWorld()->GetObjectArr().push_back(disc);
	CircleArr.push_back(disc);


	disc = new UGizmoCircleComponent(EPrimitiveColor::BLUE_Z, 0.9, "DiscZ");
	disc->SetType("CircleZ");
	disc->SetParent(this);
	disc->SetRotation(FVector(0.0f,0.0f,0.0f));
	AttachChildren.push_back(disc);
	//GetWorld()->GetObjectArr().push_back(disc);
	CircleArr.push_back(disc);

	for (auto i : CircleArr)
	{
		i->SetScale({ 2.5f,2.5f,2.5f });
	}

	obj = FObjectFactory::ConstructObject<UGizmoRectangleComponent>();
	UGizmoRectangleComponent* ScaleX = static_cast<UGizmoRectangleComponent*>(obj);
	AttachChildren.push_back(ScaleX);
	ScaleX->SetType("ScaleX");
	ScaleX->SetParent(this);
	AttachChildren.push_back(ScaleX);
	//GetWorld()->GetObjectArr().push_back(ScaleX);
	RectangleArr.push_back(ScaleX);

	obj = FObjectFactory::ConstructObject<UGizmoRectangleComponent>();
	UGizmoRectangleComponent* ScaleY = static_cast<UGizmoRectangleComponent*>(obj);
	AttachChildren.push_back(ScaleY);
	ScaleY->SetType("ScaleY");
	ScaleY->SetParent(this);
	AttachChildren.push_back(ScaleY);
	//GetWorld()->GetObjectArr().push_back(ScaleY);
	RectangleArr.push_back(ScaleY);

	obj = FObjectFactory::ConstructObject<UGizmoRectangleComponent>();
	UGizmoRectangleComponent* ScaleZ = static_cast<UGizmoRectangleComponent*>(obj);
	AttachChildren.push_back(ScaleZ);
	ScaleZ->SetType("ScaleZ");
	ScaleZ->SetParent(this);
	AttachChildren.push_back(ScaleZ);
	//GetWorld()->GetObjectArr().push_back(ScaleZ);
	RectangleArr.push_back(ScaleZ);



}

UTransformGizmo::~UTransformGizmo()
{

}

void UTransformGizmo::Initialize()
{
	Super::Initialize();
}

void UTransformGizmo::Update(double deltaTime)
{
	Super::Update(deltaTime);
	if (GetWorld()->GetPickingObj()) {
		SetLocation(GetWorld()->GetPickingObj()->GetWorldLocation());
	if (GetWorld()->GetPlayer()->GetCoordiMode() == CoordiMode::CDM_LOCAL)
	{
		
			SetRotation(GetWorld()->GetPickingObj()->GetWorldRotation());
	}
	else if (GetWorld()->GetPlayer()->GetCoordiMode() == CoordiMode::CDM_WORLD)
			SetRotation(FVector(0.0f,0.0f,0.0f));
	}
	for (int i = 0;i < 3;i++)
	{
		ArrowArr[i]->Update(deltaTime);
		CircleArr[i]->Update(deltaTime);
		RectangleArr[i]->Update(deltaTime);
	}
}

void UTransformGizmo::Release()
{
}

void UTransformGizmo::Render()
{
	for (int i = 0;i < 3;i++)
	{
		ArrowArr[i]->Render();
		CircleArr[i]->Render();
		RectangleArr[i]->Render();
	}
}
