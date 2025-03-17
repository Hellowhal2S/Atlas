#pragma once
#include "UBillboardComponent.h"
class UText :
    public UBillboardComponent
{
public:
	UText();
	~UText();
	virtual void		Initialize()				override;
	virtual void		Update(double deltaTime)	override;
	virtual void		Release()					override;
	virtual void		Render()					override;
	void				SetText(FWString _text);
	void SetRowColumnCount(int _cellsPerRow, int _cellsPerColumn);
	virtual int			CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance);

protected:

	ID3D11Buffer* vertexTextBuffer;
	TArray<FVertexTexture> vertexTextureArr;
	UINT numTextVertices;

	TArray<FVector> quad;

	const int quadSize = 2;

	int RowCount;
	int ColumnCount;

	float quadWidth = 2.0f;
	float quadHeight = 2.0f;

	void setStartUV(char alphabet, float& outStartU, float& outStartV);
	void setStartUV(wchar_t hangul, float& outStartU, float& outStartV);
	void CreateTextTextureVertexBuffer(const TArray<FVertexTexture>& _vertex,UINT byteWidth);

	void TextMVPRendering();
};

