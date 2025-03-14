#pragma once
#include "Define.h"

class UPrimitiveBatch
{
public:
	UPrimitiveBatch();
	~UPrimitiveBatch();
	static UPrimitiveBatch& GetInstance() {
		static UPrimitiveBatch instance;
		return instance;
	}

public:
	void AddLine(const FVector& start, const FVector& end, const FVector4& color);
	void AddGridLine(const FVector& start, const FVector& end, const FVector4& color);
	void AddGrid(int gridSize);
	void Begin();
	void SetSpacing(float spacing);
	void End(const FMatrix& View, const FMatrix& Projection);
	void AddBox(const FVector& center, const FVector4& color, const FMatrix& modelMatrix);
	void AddCone(const FVector& center, float radius, float height, int segments, const FVector4& color, const FMatrix& modelMatri);

	
	// ���� ������ �� ���� ������ ����
	UPrimitiveBatch(const UPrimitiveBatch&) = delete;
	UPrimitiveBatch& operator=(const UPrimitiveBatch&) = delete;
private:
	ID3D11Buffer* vertexBuffer;
	size_t allocatedCapacity;
	TArray<FVertexSimple> Vertices;
	TArray<FVertexSimple> GridVertices;
	float Spacing = 5.f;
	float PreSpacing = 0.f;
	
};