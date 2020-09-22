#include "vs_hs_transfer.hlsli"
#include "hs_ds_transfer.hlsli"

cbuffer Camera : register(b0)
{
    float3 cameraPosition;
};

cbuffer Centroid : register(b1)
{
    float3 objCentroid;
}

cbuffer LOD : register(b2)
{
    float maxTessFactor;
}

float CalculateTessFactorFromDistance(float distance)
{
    return min(maxTessFactor, max(maxTessFactor + (maxTessFactor / 4 - 1) * distance, 2));
}

TessFactors ConstFun(InputPatch<HS_IN, 3> cps, uint patchId : SV_PrimitiveID)
{
    TessFactors pt;
    float factor = CalculateTessFactorFromDistance(length(cameraPosition - objCentroid));
    
    pt.EdgeTess[0] = factor;
    pt.EdgeTess[1] = factor;
    pt.EdgeTess[2] = factor;
    pt.InsideTess[0] = factor;
    
    return pt;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstFun")]
[maxtessfactor(64.0)]
HS_OUT main(InputPatch<HS_IN, 3> cp, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUT hso;
    
    hso.position = cp[i].position;
    
    return hso;
}

