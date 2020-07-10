struct PatchConstants
{
    float EdgeTess[4] : SV_TessFactor;
    float InsideTess[2] : SV_InsideTessFactor;
    
    
};

struct HS_OUT
{
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 worldPos : POSITION;
};

typedef HS_OUT DS_IN;