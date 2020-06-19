#include "vs_hs_transfer.hlsli"

struct VS_IN
{
    float3 position : POSITION;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output;
       
    // transfers
    output.position = input.position;
    
    return output;
}