
//ワールド座標テクスチャ

//カメラ視点レジスタ b0
cbuffer cbuff0 : register(b0)
{
    float3 eyePos : packoffset(c0);
}

cbuffer cbuff1 : register(b1)
{
    float3 dirLight;
    //ライトビュープロジェクション
    //float4x4 LVPMat;
}


struct VSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
};