struct DS_OUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 worldPos : WORLDPOS;
};

typedef DS_OUT PS_IN;

//struct PS_IN
//{
//    float4 position : SV_POSITION;
//    float3 normal : NORMAL;
//    float2 texcoord : TEXCOORD;
//    float3 worldPos : WORLDPOS;
//};