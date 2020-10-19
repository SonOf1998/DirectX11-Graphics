#include "vs_gs_transfer.hlsli"
#include "gs_ps_transfer.hlsli"

cbuffer P : register(b0)
{
    matrix projMatrix;
};

cbuffer BB : register(b1)
{
    float3 rightVector;
    float3 upVector;
}

[maxvertexcount(4)]
void main(point GS_IN gin[1], uint primID : SV_PrimitiveID, inout TriangleStream<GS_OUT> tristream)
{
    float billboardWidth = 0.05f;
    float billboardHeight = 0.1f;
    
    float4 v[4];
    v[0] = float4(gin[0].position + billboardWidth * rightVector - billboardHeight * upVector, 1.0f); // jobb also
    v[1] = float4(gin[0].position - billboardWidth * rightVector - billboardHeight * upVector, 1.0f); // bal also
    v[2] = float4(gin[0].position + billboardWidth * rightVector + billboardHeight * upVector, 1.0f); // jobb felso
    v[3] = float4(gin[0].position - billboardWidth * rightVector + billboardHeight * upVector, 1.0f); // bal felso
    
    float2 t[4];
    t[0] = float2(1, 1);
    t[1] = float2(0, 1);
    t[2] = float2(1, 0);
    t[3] = float2(0, 0);
    

    GS_OUT gout;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        gout.position = mul(v[i], projMatrix);
        gout.texcoord = t[i];

        tristream.Append(gout);
    }
}