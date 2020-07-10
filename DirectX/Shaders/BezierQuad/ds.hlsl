#include "hs_ds_transfer.hlsli"
#include "ds_ps_transfer.hlsli"

cbuffer VP : register(b0)
{
    float4x4 viewProjMatrix;
}


[domain("quad")]
DS_OUT main(PatchConstants patchConstData, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUT, 4> patch)
{         
    DS_OUT dso;
    
    float3 midnormal = normalize(patch[0].normal + patch[1].normal + patch[2].normal + patch[3].normal);
    
    
    
    dso.texcoord = lerp(lerp(patch[0].texcoord, patch[1].texcoord, uv.x), lerp(patch[2].texcoord, patch[3].texcoord, uv.x), uv.y);   
    dso.worldPos = lerp ( lerp(patch[0].worldPos, patch[1].worldPos, uv.x), lerp(patch[2].worldPos, patch[3].worldPos, uv.x), uv.y);
    dso.normal   = normalize(lerp(lerp(patch[0].normal, patch[1].normal, uv.x), lerp(patch[2].normal, patch[3].normal, uv.x), uv.y));
   // dso.worldPos += 0.005 * dso.normal;
        
    dso.position = mul(float4(dso.worldPos, 1), viewProjMatrix);   
    return dso;
}