cbuffer cbuff0 : register(b0)
{
	matrix mat;  //3D変換行列
    matrix wMat;
    float4 color;//色
    matrix lightMat;
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0);   //アンビエント
	float3 m_diffuse  : packoffset(c1);   //ディフューズ
	float3 m_specular : packoffset(c2);   //スペキュラー
	float m_alpha     : packoffset(c2.w); //アルファ
}

static const int MAX_BONES = 64;

cbuffer skinnning : register(b3)
{
    matrix matSkinning[MAX_BONES];
}

struct VSInput
{
    float4 svpos        : POSITION; //システム用頂点座標
    float3 normal       : NORMAL; //法線ベクトル
    float2 uv           : TEXCOORD; //uv値
    uint4  boneIndices  : BONEINDICES;
    float4 boneWeights  : BONEWEIGHTS;
    uint    indexNum    : INDEX;
};

//頂点シェーダーからジオメトリシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos  : SV_POSITION; //システム用頂点座標
	float3 normal : NORMAL;      //法線ベクトル
	float2 uv     : TEXCOORD;    //uv値
    float4 worldPos : TEXCOORD1;
};

//ジオメトリシェーダーからピクセルシェーダーへ
struct GSOutput
{
    float4 svpos  : SV_POSITION; //システム用頂点座標（これはADSようにピクセルシェーダーでも計算する）
    float3 normal : NORMAL;      //法線ベクトル
    float2 uv     : TEXCOORD;    //uv値
    float4 worldPos : TEXCOORD1;
};

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

//ピクセルシェーダー出力構造体
struct PixelOutput
{
    float4 pixel_color	: SV_TARGET0;
    float4 normal		: SV_TARGET1;
    float4 worldPos		: SV_TARGET2;
    float4 zColor       : SV_TARGET3;   //影のみテクスチャ
};