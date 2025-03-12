#pragma once
#include "Object.h"
class UArrowComp;
class USceneComponent;
class UPlayer : public UObject
{
	DECLARE_CLASS(ULocalGizmoComponent, UObject)

public:
	UPlayer();
	virtual				~UPlayer();

	virtual void		Initialize();
	virtual void		Update(double deltaTime);
	virtual void		Release();

	void				Input();
	void PickGizmo(FVector& rayOrigin, FVector& rayDir);
	void PickObj(FVector& rayOrigin, FVector& rayDir);
	void				AddMode();

private:
	void ScreenToRay(float screenX, float screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix,
		FVector& rayOrigin, FVector& rayDir);
	bool RayIntersectsSphere(const FVector& rayOrigin, const FVector& rayDir,
		const FVector& sphereCenter, float sphereRadius);
	bool RayIntersectsObject(const FVector& rayOrigin, const FVector& rayDir, USceneComponent* obj, float& hitDistance);
	void PickedObjControl();
	void ControlRoation(UArrowComp* Arrow, USceneComponent* pObj, int32 deltaX, int32 deltaY);
	void ControlTranslation(USceneComponent* pObj, UArrowComp* Arrow, int32 deltaX, int32 deltaY);
	void ControlScale(USceneComponent* pObj, UArrowComp* Arrow, int32 deltaX, int32 deltaY);
	bool bLeftMouseDown = false;
	bool bSpaceDown = false;

	POINT m_LastMousePos;
	ControlMode cMode = CM_TRANSLATION;

public:
	void SetMode(ControlMode _Mode) { cMode = _Mode; }
	ControlMode GetMode() { return cMode; }
};

