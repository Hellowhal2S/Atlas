#pragma once
#include "UBillboardComponent.h"
class UParticleSubUVComp :
    public UBillboardComponent
{
public:
	UParticleSubUVComp();
	~UParticleSubUVComp();
	virtual void		Initialize()				override;
	virtual void		Update(double deltaTime)	override;
	virtual void		Release()					override;
	virtual void		Render()					override;

	void SetRowColumnCount(int _cellsPerRow, int _cellsPerColumn);
private:
	ID3D11Buffer* vertexSubUVBuffer;
	UINT numTextVertices;


	int CellsPerRow;
	int CellsPerColumn;

	float finalIndexU = 0.0f;
	float finalIndexV = 0.0f;

	void UpdateVertexBuffer(const TArray<FVertexTexture>& vertices);
	void CreateSubUVVertexBuffer();
};

