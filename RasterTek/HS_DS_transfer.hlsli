struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess[1] : SV_InsideTessFactor;
    
    float3 B003 : WORLDPOS0;
    float3 N003 : NORMAL0;
    float3 B030 : WORLDPOS1;
    float3 N030 : NORMAL1;
    float3 B300 : WORLDPOS2;
    float3 N300 : NORMAL2;
    
    float3 B012 : WORLDPOS3;
    float3 B021 : WORLDPOS4;
    
    float3 B120 : WORLDPOS5;
    float3 B210 : WORLDPOS6;
    
    float3 B201 : WORLDPOS7;
    float3 B102 : WORLDPOS8;
    
    float3 B111 : WORLDPOS9;
};

struct HS_OUT
{
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 worldPos : POSITION;
};

typedef HS_OUT DS_IN;