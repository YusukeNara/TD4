#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD,float4 color : COLOR,float rot : ROTATE)
{
    //取得した頂点情報をそのまま返却
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    output.color = color;
    output.rot = rot;
    return output;
}