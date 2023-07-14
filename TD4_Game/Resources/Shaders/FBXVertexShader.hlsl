#include "OBJShaderHeader.hlsli"

SkinOutput CompureSkin(VSInput input)
{
    SkinOutput output;
    
    output.normal = float3(0, 0, 0);
    output.pos = float4(0, 0, 0, 0);
	
    uint iBone;
    float weight;
    matrix m;
    
    //xyzwを擬似的な配列として使用している
	
    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul(m, float4(input.normal, 0));
	
    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul(m, float4(input.normal, 0));
	
    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul(m, float4(input.normal, 0));
	
    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul(m, float4(input.normal, 0));
	
    return output;
}

VSOutput main(VSInput input)
{
    SkinOutput skined = CompureSkin(input);
	
    float4 wn = normalize(mul(wMat, float4(skined.normal, 0)));
	
	VSOutput output;//ピクセルシェーダーに渡す値
    output.worldPos = mul(wMat, input.svpos);
	//output.svpos = mul(mat, input.svpos);
    //output.normal = normalize(mul(wMat, float4(input.normal, 0)));
    output.svpos = mul(mat, skined.pos);
    output.normal = normalize(mul(wMat, float4(skined.normal, 0)));
	output.uv = input.uv;
	return output;
}

