#include "PhongBlinn_VS_PS_transfer.hlsli"

cbuffer MVPMInv : register(b0)
{
    matrix modelMatrix;    
    matrix viewProjMatrix;
    matrix modelMatrixInv;
};

struct VS_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output;
       
    // transfers
    output.texcoord = input.texcoord;
    output.normal = mul(float4(input.normal, 0), modelMatrixInv).xyz;
    output.worldPos = mul(float4(input.position, 1), modelMatrix).xyz;
    output.position = mul(mul(float4(input.position, 1), modelMatrix), viewProjMatrix);
    
    return output;
}