// MatrixBuffer: ��ȯ ��� ����
cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 MVP;
};

// LightingBuffer: ���� ���� �Ķ���� ����
cbuffer LightingBuffer : register(b1)
{
    float3 LightDirection; // ���� ���� (���� ����; ���� ������ ������ �ݴ� ���)
    float pad1; // 16����Ʈ ���Ŀ� �е�
    float3 LightColor; // ���� ���� (��: (1, 1, 1))
    float pad2; // 16����Ʈ ���Ŀ� �е�
    float AmbientFactor; // ambient ��� (��: 0.1)
    float pad3[3]; // 16����Ʈ ���� ���� �߰� �е�
};

struct VS_INPUT
{
    float4 position : POSITION; // ���ؽ� ��ġ
    float4 color : COLOR; // ���ؽ� ����
    float3 normal : NORMAL; // ���ؽ� ���
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // ��ȯ�� ȭ�� ��ǥ
    float4 color : COLOR; // ������ ����
    float3 normal : NORMAL; // ����ȭ�� ��� ����
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    // MVP ����� ����� ��ġ ��ȯ
    output.position = mul(input.position, MVP);
    
    // ���� ����
    output.color = input.color;
    
    // ����� ����ȭ�Ͽ� ����
    output.normal = normalize(input.normal);
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // ǥ�� ��� ����ȭ
    float3 N = normalize(input.normal);
    
    // ���� ���� ��� (directional light�� ��� ���� ������ ������ �ݴ� ���)
    float3 L = normalize(-LightDirection);
    
    // Lambert ��Ģ�� ���� diffuse ��� (0~1 ���� Ŭ����)
    float diffuse = saturate(dot(N, L));
    
    // ambient�� diffuse ������ ����:
    // - ambient: AmbientFactor * �⺻ ����
    // - diffuse: diffuse * LightColor * �⺻ ����
    float3 litColor = AmbientFactor * input.color.rgb 
                    + diffuse * LightColor * input.color.rgb;
    
    return float4(litColor, input.color.a);
}
