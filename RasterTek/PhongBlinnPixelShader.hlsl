#include "PhongBlinn_VS_PS_transfer.hlsli"

Texture2D diffuseMap : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

cbuffer Material : register(b0)
{
    float3 ka;
    float4 ks;
    float shine;
};

cbuffer Camera : register(b1)
{
    float3 cameraPosition;
};

float4 main(PS_IN input) : SV_TARGET
{    
    float3 lightDir = float3(0, 1, 1);
    lightDir = normalize(lightDir);
    
    float3 viewDir = cameraPosition - input.worldPos;
    viewDir = normalize(viewDir);
    
    float3 normal = normalize(input.normal);
    
    float3 outputColor = float3(0, 0, 0);
    float3 diffuseColor = diffuseMap.Sample(samplerState, input.tex).xyz;
    
    outputColor += ka * diffuseColor;
    outputColor += saturate(dot(lightDir, normal)) * diffuseColor;
    
    float3 halfway = lightDir + viewDir;
    halfway = normalize(halfway);
    
    outputColor += ks.xyz * pow(saturate(dot(halfway, normal)), shine);
    return float4(outputColor, 1);
}
