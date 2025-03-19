Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VS_INPUT
{
    uint vertexID : SV_VertexID; // [0,3] Quad
    uint instanceID : SV_InstanceID; // 글자 하나
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 texCoord : TEXCOORD;
};

cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 MVP;
}

cbuffer SubUVOffset : register(b1)
{
    float3 uuidOrigin;
    float widthOffset;
    float heightOffset;
};

StructuredBuffer<float2> g_SubUV : register(t1);


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    float3 quad[4] =
    {
        float3(-1.0f, 1.0f, 0.0f), //LeftTop
        float3(1.0f, 1.0f, 0.0f), //RightTop
        float3(-1.0f, -1.0f, 0.0f), //LeftBottom
        float3(1.0f, -1.0f, 0.0f), //RightBottom
    };
    
    float3 vertex = quad[input.vertexID];
    vertex.x += 2 * input.instanceID;
    vertex += uuidOrigin;
    
    output.Position = mul(float4(vertex, 1.0f), MVP);    
    
    float2 quadUV[4] =
    {
        float2(0.0f, 0.0f), //LeftTop
        float2(1.0f, 0.0f), //RightTop
        float2(0.0f, 1.0f), //LeftBottom
        float2(1.0f, 1.0f)  //RightBottom
    };
    
    float2 uv = quadUV[input.vertexID];
    uv.x *= widthOffset;
    uv.y *= heightOffset;
    
    uv += g_SubUV[input.instanceID];
    output.texCoord = uv;
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    float2 uv = input.texCoord;
    float4 col = gTexture.Sample(gSampler, uv);
    
    float threshold = 0.05; // 필요한 경우 임계값을 조정
    if (col.r < threshold && col.g < threshold && col.b < threshold)
        clip(-1); // 픽셀 버리기
    
    return col;
}