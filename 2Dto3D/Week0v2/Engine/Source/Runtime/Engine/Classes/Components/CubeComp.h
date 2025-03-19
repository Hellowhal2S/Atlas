#pragma once
#include "PrimitiveComponent.h"
class UCubeComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UCubeComp, UPrimitiveComponent)

public:
	UCubeComp();
	~UCubeComp();

	virtual void		Initialize()	override;
	virtual void		Update(double deltaTime)	override;
	virtual void		Release()					override;
	virtual	void		Render()					override;
};

