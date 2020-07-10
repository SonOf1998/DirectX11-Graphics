#include "hs_ds_transfer.hlsli"
#include "ds_ps_transfer.hlsli"

cbuffer MVPMInv : register(b0)
{
    matrix modelMatrix;
    matrix viewProjMatrix;
    matrix modelMatrixInv;
};

static const float pi = 3.14159265358979323846;

[domain("tri")]
DS_OUT main(TessFactors patchConstData, float3 bary : SV_DomainLocation, const OutputPatch<HS_OUT, 3> cps)
{
    float u = bary.x;
    float v = bary.y;
    float w = bary.z;
    
    
    DS_OUT dso;
    float3 newVertex = normalize(u * cps[0].position + v * cps[1].position + w * cps[2].position);
    float texU = 0.5 + (1 / (2 * pi)) * atan2(newVertex.x, newVertex.z);
    float texV = 0.5 + (1 / pi) * asin(newVertex.y);
        
    dso.normal = mul(float4(newVertex, 0), modelMatrixInv).xyz;
    dso.texcoord = float2(texU, texV);
    dso.worldPos = mul(float4(newVertex, 1), modelMatrix).xyz;
    dso.position = mul(float4(dso.worldPos, 1), viewProjMatrix);
    return dso;
}



