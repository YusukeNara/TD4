#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	//ビュープロジェクション変換
	output.svpos = mul(mat, pos);
    output.worldPos = mul(wMat, pos);
	
    output.normal = normalize(mul(wMat, float4(normal, 0)));
	output.uv = uv;
	return output;
}