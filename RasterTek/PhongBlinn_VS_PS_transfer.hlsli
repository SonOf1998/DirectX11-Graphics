struct VS_OUT
{
    float4 position     : SV_Position;
    float3 normal       : NORMAL;
    float2 tex          : TEXCOORD;
    float3 worldPos     : WORLDPOS;
};

typedef VS_OUT PS_IN;