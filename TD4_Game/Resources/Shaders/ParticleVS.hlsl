#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD,float4 color : COLOR)
{
    //æ“¾‚µ‚½’¸“_î•ñ‚ğ‚»‚Ì‚Ü‚Ü•Ô‹p
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    output.color = color;
    return output;
}