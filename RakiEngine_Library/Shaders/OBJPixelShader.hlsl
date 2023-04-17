#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PixelOutput main(GSOutput input)
{
    PixelOutput result;
	
	float4 texColor = float4(tex.Sample(smp, input.uv));

    float3 light = normalize(float3(1, -1, 1)); //‰E‰º‰œŒü‚«ƒ‰ƒCƒg
    float diffuse = saturate(dot(-light, input.normal));
	
	float3 shade_color;
	shade_color = m_ambient;
    shade_color += m_diffuse * diffuse;
	
	float4 texcolor = tex.Sample(smp, input.uv);
	
    result.pixel_color = float4(texcolor.rgb * shade_color, texcolor.a * m_alpha) * color;
    result.normal.rgb = float3((input.normal.xyz + 1.0f) / 2.0f);
    result.normal.a = 1;
	
    return result;
}