#include "ShadowMapShaderHeader.hlsli"


VSOutput main(float4 pos : POSITION)
{
    VSOutput output; //ピクセルシェーダーに渡す値
	//ビュープロジェクション変換
    output.svpos = mul(lightMat, pos);
    
    return output;
}