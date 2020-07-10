#include "vs_ps_transfer.hlsli"

cbuffer M : register(b0)
{
    matrix modelMatrix;
};

cbuffer DirLightVP : register(b1)
{
    matrix viewProj;
}


struct VS_IN
{
    float3 position : POSITION;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output;
       
    // transfers
    output.position = mul(mul(float4(input.position, 1), modelMatrix), viewProj);
    
    return output;
}