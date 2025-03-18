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
    float2 texcoord : TEXCOORD;
};
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION; // ��ȯ�� ȭ�� ��ǥ
    float4 color : COLOR; // ������ ����
    float3 normal : NORMAL; // ����ȭ�� ��� ����
    float normalFlag : TEXCOORD0; // ��� ��ȿ�� �÷��� (1.0: ��ȿ, 0.0: ��ȿ)
    float2 texcoord : TEXCOORD1;
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    // ��ġ ��ȯ
    output.position = mul(input.position, MVP);
    output.color = input.color;
    if (flag)
        output.color *= 0.5;
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
    output.texcoord = input.texcoord;
    return output;
}

float noise(float3 p)
{
    return frac(sin(dot(p, float3(12.9898, 78.233, 37.719))) * 43758.5453);
}

float4 PaperTexture(float3 originalColor)
{
    // �Է� ������ 0~1 ������ ����
    float3 color = saturate(originalColor);
    
    float3 paperColor = float3(0.95, 0.95, 0.95);
    float blendFactor = 0.5;
    float3 mixedColor = lerp(color, paperColor, blendFactor);
    
    // ���� grain ȿ��
    float grain = noise(color * 10.0) * 0.1;
    
    // ��ģ ���� ȿ��: �� �ܰ��� ������ ���̾ ����
    float rough1 = (noise(color * 20.0) - 0.5) * 0.15;  // ù ��° ���̾�: ū �Ը��� ������
    float rough2 = (noise(color * 40.0) - 0.5) * 0.01;  // �� ��° ���̾�: ���� ���� �߰�
    float rough = rough1 + rough2;
    
    // vignette ȿ��: �߾ӿ��� �־������� ��ο����� ȿ��
    float vignetting = smoothstep(0.4, 1.0, length(color.xy - 0.5) * 2.0);
    
    // ���� ���� ���
    float3 finalColor = mixedColor + grain + rough - vignetting * 0.1;
    return float4(saturate(finalColor), 1.0);
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    // ������ ����� �ؽ�ó ������ ����
    //input.texcoord
    float4 texColor = Texture.Sample(Sampler, input.texcoord);
    //texColor = float4(1, 1, 1, 1);
    float3 color;
    if (texColor.a == 0) // �ؽ�ó�� ������ �⺻ ���� ����
    {
        color = saturate(input.color.rgb);
    }
    else
        color = saturate(input.color.rgb * texColor.rgb);

    //float3 color = saturate(input.color.rgb);

    //if (isLit == 1) // ������ ����Ǵ� ���
    //{
    //    if (input.normalFlag > 0.5)
    //    {
    //        float3 N = normalize(input.normal);
    //        float3 L = normalize(LightDirection);
    //        float diffuse = saturate(dot(N, L));
    //        color = AmbientFactor * color + diffuse * LightColor * color;
    //    }
    //    return float4(color, 1.0);
    //}
    //else // unlit ������ �� PaperTexture ȿ�� ����
    //{
    //    if (input.normalFlag < 0.5)
    //    {
    //        return float4(color, 1.0);
    //    }
    //    return PaperTexture(color);
    //}
    return float4(color, 1.0);
}
