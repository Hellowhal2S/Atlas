
cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 MVP;
};

cbuffer GridParametersData : register(b1)
{
    float GridSpacing;
    int GridCount; // �� grid ���� ��
    float3 GridOrigin; // Grid�� �߽�
    float Padding;
};
cbuffer PrimitiveCounts : register(b3)
{
    int BoundingBoxCount; // �������� AABB�� ����
    int pad;
    int ConeCount; // �������� cone�� ����
    int pad1;
};
struct FBoundingBoxData
{
    float3 bbMin;
    float padding0;
    float3 bbMax;
    float padding1;
};
struct FConeData
{
    float3 ConeApex; // ������ ������
    float ConeRadius; // ���� �ظ� ������
    
    float3 ConeBaseCenter; // ���� �ظ� �߽�
    float ConeHeight; // ���� ���� (Apex�� BaseCenter �� ����)
    float4 Color;
    
    int ConeSegmentCount; // ���� �ظ� ���� ��
    float pad[3];
};
struct FOrientedBoxCornerData
{
    float3 corners[8]; // ȸ��/�̵� �� ���� �������� 8������
};

StructuredBuffer<FBoundingBoxData> g_BoundingBoxes : register(t2);
StructuredBuffer<FConeData> g_ConeData : register(t3);
StructuredBuffer<FOrientedBoxCornerData> g_OrientedBoxes : register(t4);
static const int BB_EdgeIndices[12][2] =
{
    { 0, 1 },
    { 1, 3 },
    { 3, 2 },
    { 2, 0 }, // �ո�
    { 4, 5 },
    { 5, 7 },
    { 7, 6 },
    { 6, 4 }, // �޸�
    { 0, 4 },
    { 1, 5 },
    { 2, 6 },
    { 3, 7 } // ����
};

struct VS_INPUT
{
    uint vertexID : SV_VertexID; // 0 �Ǵ� 1: �� ������ ���۰� ��
    uint instanceID : SV_InstanceID; // �ν��Ͻ� ID�� grid, axis, bounding box�� ����
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

/////////////////////////////////////////////////////////////////////////
// Grid ��ġ ��� �Լ�
/////////////////////////////////////////////////////////////////////////
float3 ComputeGridPosition(uint instanceID, uint vertexID)
{
    int halfCount = GridCount / 2;
    float centerOffset = halfCount * 0.5; // grid �߽��� ������ ������

    float3 startPos;
    float3 endPos;
    
    if (instanceID < halfCount)
    {
        // ������: X ��ǥ ��ȭ, Y�� -centerOffset ~ +centerOffset
        float x = GridOrigin.x + (instanceID - centerOffset) * GridSpacing;
        if (abs(x - GridOrigin.x) < 0.001)
        {
            startPos = float3(0, 0, 0);
            endPos = float3(0, (GridOrigin.y - centerOffset * GridSpacing), 0);
        }
        else
        {
            startPos = float3(x, GridOrigin.y - centerOffset * GridSpacing, GridOrigin.z);
            endPos = float3(x, GridOrigin.y + centerOffset * GridSpacing, GridOrigin.z);
        }
    }
    else
    {
        // ����: Y ��ǥ ��ȭ, X�� -centerOffset ~ +centerOffset
        int idx = instanceID - halfCount;
        float y = GridOrigin.y + (idx - centerOffset) * GridSpacing;
        if (abs(y - GridOrigin.y) < 0.001)
        {
            startPos = float3(0, 0, 0);
            endPos = float3(-(GridOrigin.x + centerOffset * GridSpacing), 0, 0);
        }
        else
        {
            startPos = float3(GridOrigin.x - centerOffset * GridSpacing, y, GridOrigin.z);
            endPos = float3(GridOrigin.x + centerOffset * GridSpacing, y, GridOrigin.z);
        }

    }
    return (vertexID == 0) ? startPos : endPos;
}

/////////////////////////////////////////////////////////////////////////
// Axis ��ġ ��� �Լ� (�� 3��: X, Y, Z)
/////////////////////////////////////////////////////////////////////////
float3 ComputeAxisPosition(uint axisInstanceID, uint vertexID)
{
    float3 start = float3(0.0, 0.0, 0.0);
    float3 end;
    float zOffset = 0.f;
    if (axisInstanceID == 0)
    {
        // X ��: ������
        end = float3(1000.0, 0.0, zOffset);
    }
    else if (axisInstanceID == 1)
    {
        // Y ��: �ʷϻ�
        end = float3(0.0, 1000.0, zOffset);
    }
    else if (axisInstanceID == 2)
    {
        // Z ��: �Ķ���
        end = float3(0.0, 0.0, 1000.0 + zOffset);
    }
    else
    {
        end = start;
    }
    return (vertexID == 0) ? start : end;
}

/////////////////////////////////////////////////////////////////////////
// Bounding Box ��ġ ��� �Լ�
// bbInstanceID: StructuredBuffer���� �� ��° bounding box����
// edgeIndex: �ش� bounding box�� 12�� ���� �� ��� ������
/////////////////////////////////////////////////////////////////////////
float3 ComputeBoundingBoxPosition(uint bbInstanceID, uint edgeIndex, uint vertexID)
{
    FBoundingBoxData box = g_BoundingBoxes[bbInstanceID];
  
//    0: (bbMin.x, bbMin.y, bbMin.z)
//    1: (bbMax.x, bbMin.y, bbMin.z)
//    2: (bbMin.x, bbMax.y, bbMin.z)
//    3: (bbMax.x, bbMax.y, bbMin.z)
//    4: (bbMin.x, bbMin.y, bbMax.z)
//    5: (bbMax.x, bbMin.y, bbMax.z)
//    6: (bbMin.x, bbMax.y, bbMax.z)
//    7: (bbMax.x, bbMax.y, bbMax.z)
    int vertIndex = BB_EdgeIndices[edgeIndex][vertexID];
    float x = ((vertIndex & 1) == 0) ? box.bbMin.x : box.bbMax.x;
    float y = ((vertIndex & 2) == 0) ? box.bbMin.y : box.bbMax.y;
    float z = ((vertIndex & 4) == 0) ? box.bbMin.z : box.bbMax.z;
    return float3(x, y, z);
}

/////////////////////////////////////////////////////////////////////////
// Axis ��ġ ��� �Լ� (�� 3��: X, Y, Z)
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
// Cone ��� �Լ�
/////////////////////////////////////////////////
// Helper: ���� ������ ���� �ظ� ������ ��ġ ���

float3 ComputeConePosition(uint globalInstanceID, uint vertexID)
{
    // ��� cone�� ������ ���׸�Ʈ ���� ����
    int N = g_ConeData[0].ConeSegmentCount;
    
    uint coneIndex = globalInstanceID / (2 * N);
    uint lineIndex = globalInstanceID % (2 * N);
    
    // cone ������ �б�
    FConeData cone = g_ConeData[coneIndex];
    
    // cone�� �� ���
    float3 axis = normalize(cone.ConeApex - cone.ConeBaseCenter);
    
    // axis�� ������ �� ����(u, v)�� ����
    float3 arbitrary = abs(dot(axis, float3(0, 0, 1))) < 0.99 ? float3(0, 0, 1) : float3(0, 1, 0);
    float3 u = normalize(cross(axis, arbitrary));
    float3 v = cross(axis, u);
    
    if (lineIndex < (uint) N)
    {
        // ���� ����: cone�� �������� �ظ��� �� ���� �մ´�.
        float angle = lineIndex * 6.28318530718 / N;
        float3 baseVertex = cone.ConeBaseCenter + (cos(angle) * u + sin(angle) * v) * cone.ConeRadius;
        return (vertexID == 0) ? cone.ConeApex : baseVertex;
    }
    else
    {
        // �ظ� �ѷ� ����: �ظ���� ������ �� ���� �մ´�.
        uint idx = lineIndex - N;
        float angle0 = idx * 6.28318530718 / N;
        float angle1 = ((idx + 1) % N) * 6.28318530718 / N;
        float3 v0 = cone.ConeBaseCenter + (cos(angle0) * u + sin(angle0) * v) * cone.ConeRadius;
        float3 v1 = cone.ConeBaseCenter + (cos(angle1) * u + sin(angle1) * v) * cone.ConeRadius;
        return (vertexID == 0) ? v0 : v1;
    }
}
/////////////////////////////////////////////////////////////////////////
// OBB
/////////////////////////////////////////////////////////////////////////
float3 ComputeOrientedBoxPosition(uint obIndex, uint edgeIndex, uint vertexID)
{
    FOrientedBoxCornerData ob = g_OrientedBoxes[obIndex];
    int cornerID = BB_EdgeIndices[edgeIndex][vertexID];
    return ob.corners[cornerID];
}

/////////////////////////////////////////////////////////////////////////
// ���� ���ؽ� ���̴�
/////////////////////////////////////////////////////////////////////////
PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    float3 pos;
    float4 color;
    
    // Cone �ϳ��� (2 * SegmentCount) ����.
    // ConeCount ������ŭ�̹Ƿ� �� (2 * SegmentCount * ConeCount).
    uint coneInstCnt = ConeCount * 2 * g_ConeData[0].ConeSegmentCount;

    // Grid / Axis / AABB �ν��Ͻ� ���� ���
    uint gridLineCount = GridCount; // �׸��� ����
    uint axisCount = 3; // X, Y, Z �� (���� ��ǥ��)
    uint aabbInstanceCount = 12 * BoundingBoxCount; // AABB �ϳ��� 12�� ����

    // 1) "�� �ν��Ͻ� ����" ����
    uint coneInstanceStart = gridLineCount + axisCount + aabbInstanceCount;
    // 2) �� ����(=�� ������ ��)�� �� OBB ���� ����
    uint obbStart = coneInstanceStart + coneInstCnt;

    // ���� instanceID�� �������� �б�
    if (input.instanceID < gridLineCount)
    {
        // 0 ~ (GridCount-1): �׸���
        pos = ComputeGridPosition(input.instanceID, input.vertexID);
        color = float4(0.1, 0.1, 0.1, 1.0);
    }
    else if (input.instanceID < gridLineCount + axisCount)
    {
        // �� ���� (axisCount)��: ��(Axis)
        uint axisInstanceID = input.instanceID - gridLineCount;
        pos = ComputeAxisPosition(axisInstanceID, input.vertexID);

        // �ึ�� ����
        if (axisInstanceID == 0)
            color = float4(1.0, 0.0, 0.0, 1.0); // X: ����
        else if (axisInstanceID == 1)
            color = float4(0.0, 1.0, 0.0, 1.0); // Y: �ʷ�
        else
            color = float4(0.0, 0.0, 1.0, 1.0); // Z: �Ķ�
    }
    else if (input.instanceID < gridLineCount + axisCount + aabbInstanceCount)
    {
        // �� ���� AABB �ν��Ͻ� ����
        uint index = input.instanceID - (gridLineCount + axisCount);
        uint bbInstanceID = index / 12; // 12���� 1�ڽ�
        uint bbEdgeIndex = index % 12;
        
        pos = ComputeBoundingBoxPosition(bbInstanceID, bbEdgeIndex, input.vertexID);
        color = float4(1.0, 1.0, 0.0, 1.0); // �����
    }
    else if (input.instanceID < obbStart)
    {
        // �� ���� ��(Cone) ����
        uint coneInstanceID = input.instanceID - coneInstanceStart;
        pos = ComputeConePosition(coneInstanceID, input.vertexID);
        int N = g_ConeData[0].ConeSegmentCount;
        uint coneIndex = coneInstanceID / (2 * N);
        
        color = g_ConeData[coneIndex].Color;
   
        
    }
    else
    {
        uint obbLocalID = input.instanceID - obbStart;
        uint obbIndex = obbLocalID / 12;
        uint edgeIndex = obbLocalID % 12;

        pos = ComputeOrientedBoxPosition(obbIndex, edgeIndex, input.vertexID);
        color = float4(0.4, 1.0, 0.4, 1.0); // ����: ���λ�
    }

    // ��� ��ȯ
    output.Position = mul(float4(pos, 1.0), MVP);
    output.Color = color;
    return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    return input.Color;
}
