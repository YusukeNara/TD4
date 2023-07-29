
//ワールド座標テクスチャ

//カメラ視点レジスタ b0
cbuffer cbuff0 : register(b0)
{
    float3 eyePos : packoffset(c0);
}

struct DirectionalLight
{
    float4 lightDir;
    uint isActive;
    uint isUseSpecular;
};

cbuffer cbuff1 : register(b1)
{
    DirectionalLight lightStatus[4];
}


struct VSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
};