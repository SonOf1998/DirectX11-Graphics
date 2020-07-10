struct TessFactors
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess[1] : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 position : POSITION;
};

typedef HS_OUT DS_IN;