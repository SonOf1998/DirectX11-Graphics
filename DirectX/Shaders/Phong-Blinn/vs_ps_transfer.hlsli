struct VS_OUT
{
    float4 position     : SV_Position;
    float3 normal       : NORMAL;
    float2 texcoord     : TEXCOORD;
    float3 worldPos     : WORLDPOS;
    float3 posFromLight : POS2;
};

typedef VS_OUT PS_IN;