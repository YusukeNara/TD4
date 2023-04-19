#include "Sprite.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    //float distotion = 0.00;
    ////òpã»ï\åª
    //float2 samplePoint = input.uv;
    //samplePoint -= float2(0.5f, 0.5f);
    //float power = pow(length(samplePoint), distotion);
    //samplePoint *= power;
    //samplePoint += float2(0.5f, 0.5f);
    //float4 color = tex.Sample(smp, samplePoint) * input.color;
    
    float4 color = tex.Sample(smp, input.uv) * input.color;
	
    //ÉZÉsÉAí≤â¡çH
    float bright = (color.r + color.g + color.b) * 0.33f;
    float3 sepia;
    sepia.r = bright * input.freedata.r;
    sepia.g = bright * input.freedata.g;
    sepia.b = bright * input.freedata.b;
    color.rgb = lerp(color.rgb, sepia, input.freedata.a);
    
    return color;
}