#include "VS_HS_transfer.hlsli"

cbuffer MMInv : register(b0)
{
    matrix modelMatrix;
    matrix modelMatrixInv;
};

struct VS_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output;
       
    // transfers
    output.tex = input.tex;
    output.normal = mul(float4(input.normal, 0), modelMatrixInv).xyz;
    output.worldPos = mul(float4(input.position, 1), modelMatrix).xyz;
    
    return output;
}