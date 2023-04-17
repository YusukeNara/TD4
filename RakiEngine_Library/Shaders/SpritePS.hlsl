#include "Sprite.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv) * color;
}