#include "ParticleShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

//PSOutput main(GSOutput input)
//{
//    PSOutput output;
//    output.pixel_color = tex.Sample(smp, input.uv) * input.color;
//    output.worldPos = input.svpos;
//    output.normal = float4(0, 0, 0, 0);
//    return output;
//}

float4 main(GSOutput input) : SV_Target{
    return tex.Sample(smp, input.uv) * input.color;

}