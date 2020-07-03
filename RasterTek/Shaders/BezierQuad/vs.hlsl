#include "vs_hs_transfer.hlsli"

cbuffer MMInv : register(b0)
{
    matrix modelMatrix[50];
    matrix modelMatrixInv[50];
};

struct VS_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    uint InstanceID : SV_InstanceID;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output;
       
    // transfers
    output.texcoord = input.texcoord;
    output.normal = mul(float4(input.normal, 0), modelMatrixInv[input.InstanceID]).xyz;
    output.worldPos = mul(float4(input.position, 1), modelMatrix[input.InstanceID]).xyz;
    
    return output;
}