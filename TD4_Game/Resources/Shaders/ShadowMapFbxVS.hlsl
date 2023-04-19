#include "ShadowMapShaderHeader.hlsli"

SkinOutput CompureSkin(fbxVSInput input)
{
    SkinOutput output;
    
    output.pos = float4(0, 0, 0, 0);
	
    uint iBone;
    float weight;
    matrix m;
	
    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
	
    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
	
    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
	
    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
	
    return output;
}

VSOutput main(fbxVSInput input)
{
    SkinOutput skined = CompureSkin(input);
	
    VSOutput output; //ピクセルシェーダーに渡す値
    output.svpos = mul(lightMat, skined.pos);
    return output;
}