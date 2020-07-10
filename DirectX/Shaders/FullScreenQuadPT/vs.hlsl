#include "vs_ps_transfer.hlsli"

cbuffer MVP : register(b0)
{
	matrix modelMatrix;
	matrix viewProjMatrix;
};

struct VS_IN
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};


VS_OUT main(VS_IN input)
{
	VS_OUT output;
       
    // transfers
	output.texcoord = input.texcoord;
	output.position = mul(float4(input.position, 1), modelMatrix);
    
	return output;
}