#pragma once
#include "SceneComponent.h"
#include <windows.h>

class UCameraComponent :
    public USceneComponent
{
	DECLARE_CLASS(UCameraComponent, USceneComponent)

public:
	UCameraComponent();
	~UCameraComponent();

	virtual void		Initialize()	override;
	virtual void		Update(double deltaTime)		override;
	virtual void		Release()					override;

	void				Input();
	void				MoveForward(float _Value);
	void				MoveRight(float _Value);
	void				MoveUp(float _Value);
	void				RotateYaw(float _Value);
	void				RotatePitch(float _Value);

private:
	float m_mouseSpeed = 0.25f;
	POINT m_LastMousePos;
	bool bRightMouseDown = false;
	FVector zAxis;  // DirectX�� LH�̹Ƿ� -z�� �ƴ϶� +z ���
	FVector xAxis;
	FVector yAxis;
	float fov;
public:
	void SetFov(float _fov) { fov = _fov; }
	float& GetFov() { return fov; }

};

