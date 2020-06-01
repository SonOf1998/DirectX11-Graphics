#include "FullScreenQuadPT_VS_PS_transfer.hlsli"

Texture2D lastPassMap : TEXTURE0 : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    float3 outputColor = lastPassMap.Sample(samplerState, input.texcoord);    
    return float4(outputColor, 1);
}
