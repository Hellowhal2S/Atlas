// MatrixBuffer: 변환 행렬 관리
cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 MVP;
    float flag;
};

// LightingBuffer: 조명 관련 파라미터 관리
cbuffer LightingBuffer : register(b1)
{
    float3 LightDirection; // 조명 방향 (단위 벡터; 빛이 들어오는 방향의 반대 사용)
    float pad1; // 16바이트 정렬용 패딩
    float3 LightColor; // 조명 색상 (예: (1, 1, 1))
    float pad2; // 16바이트 정렬용 패딩
    float AmbientFactor; // ambient 계수 (예: 0.1)
    float pad3; // 16바이트 정렬 맞춤 추가 패딩
    float pad4; // 16바이트 정렬 맞춤 추가 패딩
    float pad5; // 16바이트 정렬 맞춤 추가 패딩
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
    float4 position : POSITION; // 버텍스 위치
    float4 color : COLOR; // 버텍스 색상
    float3 normal : NORMAL; // 버텍스 노멀
    float2 texcoord : TEXCOORD;
};
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION; // 변환된 화면 좌표
    float4 color : COLOR; // 전달할 색상
    float3 normal : NORMAL; // 정규화된 노멀 벡터
    float normalFlag : TEXCOORD0; // 노멀 유효성 플래그 (1.0: 유효, 0.0: 무효)
    float2 texcoord : TEXCOORD1;
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    // 위치 변환
    output.position = mul(input.position, MVP);
    output.color = input.color;
    if (flag)
        output.color *= 0.5;
    // 입력 normal 값의 길이 확인
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

float noise(float3 p)
{
    return frac(sin(dot(p, float3(12.9898, 78.233, 37.719))) * 43758.5453);
}

float4 PaperTexture(float3 originalColor)
{
    // 입력 색상을 0~1 범위로 제한
    float3 color = saturate(originalColor);
    
    float3 paperColor = float3(0.95, 0.95, 0.95);
    float blendFactor = 0.5;
    float3 mixedColor = lerp(color, paperColor, blendFactor);
    
    // 정적 grain 효과
    float grain = noise(color * 10.0) * 0.1;
    
    // 거친 질감 효과: 두 단계의 노이즈 레이어를 결합
    float rough1 = (noise(color * 20.0) - 0.5) * 0.15;  // 첫 번째 레이어: 큰 규모의 노이즈
    float rough2 = (noise(color * 40.0) - 0.5) * 0.01;  // 두 번째 레이어: 세부 질감 추가
    float rough = rough1 + rough2;
    
    // vignette 효과: 중앙에서 멀어질수록 어두워지는 효과
    float vignetting = smoothstep(0.4, 1.0, length(color.xy - 0.5) * 2.0);
    
    // 최종 색상 계산
    float3 finalColor = mixedColor + grain + rough - vignetting * 0.1;
    return float4(saturate(finalColor), 1.0);
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    float3 color = saturate(input.color.rgb);

    if (isLit == 1) // 조명이 적용되는 경우
    {
        if (input.normalFlag > 0.5)
        {
            float3 N = normalize(input.normal);
            float3 L = normalize(LightDirection);
            float diffuse = saturate(dot(N, L));
            color = AmbientFactor * color + diffuse * LightColor * color;
        }
        return float4(color, 1.0);
    }
    else // unlit 상태일 때 PaperTexture 효과 적용
    {
        if (input.normalFlag < 0.5)
        {
            return float4(color, 1.0);
        }
        return PaperTexture(color);
    }
}
