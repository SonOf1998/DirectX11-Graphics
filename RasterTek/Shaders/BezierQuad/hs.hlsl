#include "vs_hs_transfer.hlsli"
#include "hs_ds_transfer.hlsli"

PatchConstants ConstFun(InputPatch<HS_IN, 4> cps, uint patchId : SV_PrimitiveID)
{
    PatchConstants pc;
    pc.EdgeTess[0]      = 1;
    pc.EdgeTess[1]      = 1;
    pc.EdgeTess[2]      = 1;
    pc.EdgeTess[3]      = 1;
    pc.InsideTess[0]    = 1;
    pc.InsideTess[1]    = 1;
    
    return pc;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstFun")]
[maxtessfactor(64.0)]
HS_OUT main(InputPatch<HS_IN, 4> cp, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUT hso;
    
    hso.worldPos = cp[i].worldPos;
    hso.normal   = cp[i].normal;
    hso.texcoord = cp[i].texcoord;    
    
    return hso;
}


