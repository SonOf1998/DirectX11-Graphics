#include "vs_gs_transfer.hlsli"

cbuffer M : register(b0)
{
	matrix modelMatrix;
};

struct VS_IN
{
	float3 position : POSITION;
};


VS_OUT main(VS_IN input)
{
	VS_OUT output;
       
    // transfers
	output.position = mul(float4(input.position, 1), modelMatrix).xyz;
    
	return output;
}