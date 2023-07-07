cbuffer cbuff0 : register(b0)
{
    matrix mat; //3D変換行列
    float4 color; //色
    matrix lightMat;
}

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); //アンビエント
    float3 m_diffuse : packoffset(c1); //ディフューズ
    float3 m_specular : packoffset(c2); //スペキュラー
    float m_alpha : packoffset(c2.w); //アルファ
}

static const int MAX_BONES = 64;

cbuffer skinnning : register(b4)
{
    matrix matSkinning[MAX_BONES];
}

struct VSInput
{
    float4 svpos        : POSITION; //システム用頂点座標
};

struct fbxVSInput
{
    float4  svpos           : POSITION; //システム用頂点座標
    uint4   boneIndices     : BONEINDICES;
    float4  boneWeights     : BONEWEIGHTS;
};

//頂点シェーダーからジオメトリシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos        : SV_POSITION; //システム用頂点座標
};

struct SkinOutput
{
    float4 pos;
};

//ピクセルシェーダー出力構造体
struct PixelOutput
{
    float4 depthColor   : SV_TARGET;
};