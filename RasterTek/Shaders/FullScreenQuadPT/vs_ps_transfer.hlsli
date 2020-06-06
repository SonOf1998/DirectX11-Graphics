struct VS_OUT
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD;
    float3 worldPos : WORLDPOS;
};

typedef VS_OUT PS_IN;