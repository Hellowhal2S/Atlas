#include "PrimitiveBatch.h"
#include "EngineLoop.h"
UPrimitiveBatch::UPrimitiveBatch()
{
}

UPrimitiveBatch::~UPrimitiveBatch()
{
    if (vertexBuffer) {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
}

void UPrimitiveBatch::AddLine(const FVector& start, const FVector& end, const FVector4& color)
{
	Vertices.push_back({ start.x, start.y, start.z, 
						 color.x, color.y, color.z, 1 });

	Vertices.push_back({ end.x, end.y, end.z,
						 color.x, color.y, color.z, 1 });
}

void UPrimitiveBatch::Begin()
{
	Vertices.clear();
}

void UPrimitiveBatch::AddGrid(int gridSize)
{
    for (int i = -gridSize; i <= gridSize; i++)
    {
        float pos = i * Spacing;
        
        // y-up ����
        // ���μ�
        AddLine({ pos, -gridSize * Spacing, 0 }, { pos, gridSize * Spacing, 0 }, { 0.5f, 0.5f, 0.5f, 1.0f });

        // ���μ�
        AddLine({ -gridSize * Spacing,  pos, 0 }, { gridSize * Spacing,  pos, 0 }, { 0.5f, 0.5f, 0.5f, 1.0f });
    }
}
void UPrimitiveBatch::SetSpacing(float spacing)
{
    Spacing = spacing;
}

void UPrimitiveBatch::End(const FMatrix& View, const FMatrix& Projection)
{
    if (Vertices.empty()) return; // �������� �����Ͱ� ������ �н�

    if (Vertices.size() > allocatedCapacity) {
        if (vertexBuffer) {
            vertexBuffer->Release();
        }
        allocatedCapacity = Vertices.size();
        vertexBuffer = FEngineLoop::renderer.CreateDynamicBatchBuffer(allocatedCapacity);
    }

    FEngineLoop::renderer.UpdateBuffer(vertexBuffer, Vertices);


    UINT stride = sizeof(FVertexSimple);
    UINT offset = 0;
    FMatrix Model = FMatrix::Identity;

    FMatrix MVP = Model * View * Projection;
    FEngineLoop::renderer.UpdateConstant(MVP, 0.0f);
    FEngineLoop::renderer.RenderBatch(vertexBuffer, Vertices.size(), stride, offset);

    Vertices.clear();
}

void UPrimitiveBatch::AddBox(const FVector& center, const FVector4& color, const FMatrix& modelMatrix)
{  
    FVector half = {1,1,1};
    FVector localVertices[8] = {
        FVector(-half.x, -half.y, -half.z),
        FVector(half.x, -half.y, -half.z),
        FVector(half.x,  half.y, -half.z),
        FVector(-half.x,  half.y, -half.z),
        FVector(-half.x, -half.y,  half.z),
        FVector(half.x, -half.y,  half.z),
        FVector(half.x,  half.y,  half.z),
        FVector(-half.x,  half.y,  half.z)
    };

    // �� ����� ���� ���� ��ǥ�� ��ȯ
    FVector worldVertices[8];
    for (int i = 0; i < 8; i++) {
        worldVertices[i] = center + FMatrix::TransformVector(localVertices[i], modelMatrix);
    }

    // �ظ� ����: 0-1, 1-2, 2-3, 3-0
    AddLine(worldVertices[0], worldVertices[1], color);
    AddLine(worldVertices[1], worldVertices[2], color);
    AddLine(worldVertices[2], worldVertices[3], color);
    AddLine(worldVertices[3], worldVertices[0], color);

    // ���� ����: 4-5, 5-6, 6-7, 7-4
    AddLine(worldVertices[4], worldVertices[5], color);
    AddLine(worldVertices[5], worldVertices[6], color);
    AddLine(worldVertices[6], worldVertices[7], color);
    AddLine(worldVertices[7], worldVertices[4], color);

    // ���� ����: 0-4, 1-5, 2-6, 3-7
    AddLine(worldVertices[0], worldVertices[4], color);
    AddLine(worldVertices[1], worldVertices[5], color);
    AddLine(worldVertices[2], worldVertices[6], color);
    AddLine(worldVertices[3], worldVertices[7], color);
}


void UPrimitiveBatch::AddCone(const FVector& center, float radius, float height, int segments, const FVector4& color, const FMatrix& modelMatrix)
{
    // ���� ��ǥ���� �������� (0,0,0)���� ���ǵ�
    FVector localApex = FVector(0, 0, 0);
    // ���� ��ǥ���� ������ (�� ��� ��ȯ �� center ������ ����)
    FVector worldApex = center + FMatrix::TransformVector(localApex, modelMatrix);

    float angleStep = 2 * PI / segments;
    TArray<FVector> basePoints;
    basePoints.reserve(segments);

    // �ظ� ���� �� ���� ���� ��ǥ���� ��� (z = height���� ���� ����)
    for (int i = 0; i < segments; i++)
    {
        float angle = i * angleStep;
        // ���� ��ǥ���� �ظ� ��: z ��ǥ�� height
        FVector localPoint = FVector(cos(angle) * radius, sin(angle) * radius, height);
        // ���� ��ǥ�� ��ȯ
        basePoints.push_back(center + FMatrix::TransformVector(localPoint, modelMatrix));
    }

    // �ظ� �� �ѷ��� �̷�� ���� �߰�
    for (int i = 0; i < segments; i++)
    {
        int next = (i + 1) % segments;
        AddLine(basePoints[i], basePoints[next], color);
    }

    // ������(���彺)�� �ظ� �� ���� �����ϴ� ���� �߰� (����)
    for (int i = 0; i < segments; i++)
    {
        AddLine(worldApex, basePoints[i], color);
    }
}
