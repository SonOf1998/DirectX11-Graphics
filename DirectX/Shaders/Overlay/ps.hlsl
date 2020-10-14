#include "vs_ps_transfer.hlsli"

Texture2D overlayTexture : TEXTURE0 : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    float4 outputColor = overlayTexture.Sample(samplerState, input.texcoord).xyzw;    
    return outputColor;
}
