#include "ShadowMapShaderHeader.hlsli"

PixelOutput main(VSOutput input)
{
    PixelOutput resultColor;
    
    float z = input.svpos.z;
    
    resultColor.depthColor =  float4(z, z, z, 1.0f);
	
	return resultColor;
}