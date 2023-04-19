#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	//頂点座標そのまま
    output.worldPos = pos;
	//ビュープロジェクション変換
	output.svpos = mul(mat, pos);
    output.worldPos = mul(wMat, pos);
	
	output.normal = normal;
	output.uv = uv;
	return output;
}