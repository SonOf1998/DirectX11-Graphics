#include "vs_ps_transfer.hlsli"

Texture2D lastPassMap : TEXTURE0 : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    float outputColor = lastPassMap.Sample(samplerState, input.texcoord).x;    
    return float4(outputColor, outputColor, outputColor, 1);
}
