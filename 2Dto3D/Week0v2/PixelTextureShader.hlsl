Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET {

    float4 col = gTexture.Sample(gSampler, input.texCoord);
    float threshold = 0.05; // �ʿ��� ��� �Ӱ谪�� ����
    if (col.r < threshold && col.g < threshold && col.b < threshold)
        clip(-1); // �ȼ� ������
    
    return col;
}
