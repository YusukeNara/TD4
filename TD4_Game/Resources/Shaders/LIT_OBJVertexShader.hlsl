#include "LIT_OBJHeader.hlsli"

VSOutput main(float3 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos.xyz = pos;
    output.svpos.w = 1.0f;
    output.uv = uv;
    float3 eye = eyePos;
    
    return output;
}