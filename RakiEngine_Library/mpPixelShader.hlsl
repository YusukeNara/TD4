#include "mpShaderHeader.hlsli"

float4 ps(Output input) : SV_TARGET
{
    return float4(input.uv, 1, 1);
}