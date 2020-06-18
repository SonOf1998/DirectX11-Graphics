#include "VS_HS_transfer.hlsli"
#include "HS_DS_transfer.hlsli"

cbuffer Camera : register(b0)
{
    float3 cameraPosition;
};

float CalculateTessFactorFromDistance(float distance)
{
    return max(64 - distance * 0.3, 1);
}

TessFactors ConstFun(InputPatch<HS_IN, 3> cps, uint patchId : SV_PrimitiveID)
{
    TessFactors pt;
    pt.EdgeTess[0] = CalculateTessFactorFromDistance(length(cameraPosition - cps[0].worldPos));
    pt.EdgeTess[1] = CalculateTessFactorFromDistance(length(cameraPosition - cps[1].worldPos));
    pt.EdgeTess[2] = CalculateTessFactorFromDistance(length(cameraPosition - cps[2].worldPos));
    
    float3 patchCentroid = (cps[0].worldPos + cps[1].worldPos + cps[2].worldPos) / 3.0;
    
    pt.InsideTess[0] = CalculateTessFactorFromDistance(length(cameraPosition - patchCentroid));
    
    return pt;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstFun")]
[maxtessfactor(64.0)]
HS_OUT main(InputPatch<HS_IN, 3> cp, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUT hso;
    
    hso.position = cp[i].position;
    
    return hso;
}

