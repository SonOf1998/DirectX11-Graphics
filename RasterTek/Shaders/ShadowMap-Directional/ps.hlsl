#include "vs_ps_transfer.hlsli"

float main(PS_IN input) : SV_TARGET
{
    return input.position.z / input.position.w;
}
