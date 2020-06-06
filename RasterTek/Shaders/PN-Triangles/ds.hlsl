#include "HS_DS_transfer.hlsli"
#include "DS_PS_transfer.hlsli"

cbuffer ViewProj : register(b0)
{
    float4x4 viewProjMatrix;
}

float2 interpolateTexture(float2 t1, float2 t2, float2 t3, float3 bary)
{
    return float2(bary.x, bary.x) * t1 + float2(bary.y, bary.y) * t2 + float2(bary.z, bary.z) * t3;
}

float3 interpolateNormal(float3 n1, float3 n2, float3 n3, float3 bary)
{
    return float3(bary.x, bary.x, bary.x) * n1 + float3(bary.y, bary.y, bary.y) * n2 + float3(bary.z, bary.z, bary.z) * n3;
}

[domain("tri")]
DS_OUT main(PatchTess patchConstData, float3 bary : SV_DomainLocation, const OutputPatch<HS_OUT, 3> patch)
{
    float u = bary.x;
    float v = bary.y;
    float w = bary.z;
    
    
    DS_OUT dso;
    dso.tex = interpolateTexture(patch[0].tex, patch[1].tex, patch[2].tex, bary);
    dso.normal = interpolateNormal(patch[0].normal, patch[1].normal, patch[2].normal, bary);
    dso.worldPos = patchConstData.B300 * w * w * w +
                   patchConstData.B030 * u * u * u +
                   patchConstData.B003 * v * v * v +
                   patchConstData.B210 * 3.0 * w * w * u +
                   patchConstData.B120 * 3.0 * w * u * u +
                   patchConstData.B201 * 3.0 * w * w * v +
                   patchConstData.B021 * 3.0 * u * u * v +
                   patchConstData.B102 * 3.0 * w * v * v +
                   patchConstData.B012 * 3.0 * u * v * v +
                   patchConstData.B111 * 6.0 * w * u * v;
    
    dso.position = mul(float4(dso.worldPos, 1), viewProjMatrix);   
    return dso;
}