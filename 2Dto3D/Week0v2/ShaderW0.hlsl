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
    float pad3; // 16����Ʈ ���� ���� �߰� �е�
    float pad4; // 16����Ʈ ���� ���� �߰� �е�
    float pad5; // 16����Ʈ ���� ���� �߰� �е�
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
    float normalFlag : TEXCOORD0; // ��� ��ȿ�� �÷��� (1.0: ��ȿ, 0.0: ��ȿ)
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    // ��ġ ��ȯ
    output.position = mul(input.position, MVP);
    output.color = input.color;
    
    // �Է� normal ���� ���� Ȯ��
    float normalThreshold = 0.001;
    float normalLen = length(input.normal);
    
    if (normalLen < normalThreshold)
    {
        output.normalFlag = 0.0;
    }
    else
    {
        //output.normal = normalize(input.normal);
        output.normal = mul(input.normal, MVP);
        output.normalFlag = 1.0;
    }
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // normal ���� ��ȿ���� ������ ����� ���
    if (input.normalFlag < 0.5)
    {
        return float4(input.color.rgb, input.color.a);
    }
    
    // �������� normal ���� ���� �� ���� ��� ����
    float3 N = normalize(input.normal);
    float3 L = normalize(LightDirection);
    float diffuse = saturate(dot(N, L));

    // ambient�� diffuse ������ ����
    float3 litColor = AmbientFactor * input.color.rgb 
                    + diffuse * LightColor * input.color.rgb;
    
    return float4(litColor, input.color.a);
}
