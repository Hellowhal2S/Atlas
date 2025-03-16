// MatrixBuffer: ��ȯ ��� ����
cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 MVP;
    float flag;
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
cbuffer FNormalConstants : register(b2)
{
    row_major float4x4 MInverseTranspose;
};
cbuffer FLitUnlitConstants : register(b3)
{
    int isLit;
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
    if (flag)
        output.color += 0.5;
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
        output.normal = mul(input.normal, MInverseTranspose);
        output.normalFlag = 1.0;
    }
    
    return output;
}


float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float3 color = input.color.rgb;

    //if (isLit == 1) // normal�� ��ȿ�� ���� ���� ����
    //{
    //    if (input.normalFlag > 0.5)
    //    {
    //        float3 N = normalize(input.normal);
    //        float3 L = normalize(LightDirection);
    //        float diffuse = saturate(dot(N, L));
    //        color = AmbientFactor * color + diffuse * LightColor * color;
    //    }
        
    //}
    //else
    //{
    //    if (input.normalFlag > 0.5)
    //    {
    //        float lv = 3.0;
    //        color = floor(color * lv) / (lv - 1);
    //        color = lerp(float3(0.5, 0.5, 0.5), color, 0.4); // �� �ٿ�
    //    }
    //}

    return float4(color, 1.0);
}
