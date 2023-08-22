#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//通常空間をライト空間に変換し、zを比較、影テクスチャを生成する
float4 MakeShadowTexture(float4x4 lvpm, float4 cvpPos, float4 lvpPos,float4 albedo)
{
    float4 result;
    result.w = albedo.w;
    
    cvpPos = mul(lvpm, cvpPos);
    
    if (cvpPos.z > lvpPos.z)
    {
        result.xyz = 0.5;
    }
    
    return result;
}

PixelOutput main(GSOutput input)
{
    PixelOutput result;

	float4 texcolor = tex.Sample(smp, input.uv);
	
    result.pixel_color = float4(texcolor.rgb * baseColor, 1.0f );
    result.normal.xyz = float3(input.normal.xyz / 2.0f) + 0.5f;
    result.normal.w = texcolor.a;
    result.worldPos = input.worldPos;
    
    //通常カメラ空間z値
    float4 cz = float4(input.svpos.z, input.svpos.z, input.svpos.z, 1.0f);
    
    //ピクセルのライト空間のz値（シャドウマップ）
    float4 lvppos = mul(lightMat, input.worldPos);
    float4 lz = float4(lvppos.z, lvppos.z, lvppos.z, 1.0f);
    
    //ライト空間zより低い値の場合は影
    //result.zColor = MakeShadowTexture(lightMat, cz, lvppos, result.pixel_color);
    result.zColor = float4(texcolor.rgb, 1.0f);
    
    result.metalness = float4(PBRParams.x, 0, 0, 1);
    result.specular = float4(PBRParams.y, 0, 0, 1);
    result.roughness = float4(PBRParams.z, 0, 0, 1);
    
    return result;
}

