#include "vs_ps_transfer.hlsli"

Texture2D diffuseMap : TEXTURE0 : register(t0);
Texture2D shadowMap : TEXTURE1 : register(t1);
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
    float3 diffuseColor = diffuseMap.Sample(samplerState, input.texcoord).xyz;
    
    outputColor += ka * diffuseColor;
    outputColor += saturate(dot(lightDir, normal)) * diffuseColor;
    
    float3 halfway = lightDir + viewDir;
    halfway = normalize(halfway);
    
    outputColor += ks.xyz * pow(saturate(dot(halfway, normal)), shine);
    
    float2 projTexCoord = float2(input.posFromLight.x / 2 + 0.5, -input.posFromLight.y / 2 + 0.5);
    float depth = shadowMap.Sample(samplerState, projTexCoord).x;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    if (depth < input.posFromLight.z - bias)
    {
        outputColor *= 0.7;
    }
   
    float3 a = float3(depth, depth, depth);
    float3 b = float3(input.posFromLight.z, input.posFromLight.z, input.posFromLight.z);
    
    return float4(outputColor, 1);
}
