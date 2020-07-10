struct VS_OUT
{
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 worldPos : POSITION;
};

struct HS_IN
{
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 worldPos : POSITION;
};