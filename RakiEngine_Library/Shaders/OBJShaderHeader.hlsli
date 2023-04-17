cbuffer cbuff0 : register(b0)
{
	matrix mat;  //3D変換行列
    float4 color;//色
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0);   //アンビエント
	float3 m_diffuse  : packoffset(c1);   //ディフューズ
	float3 m_specular : packoffset(c2);   //スペキュラー
	float m_alpha     : packoffset(c2.w); //アルファ
}

//頂点シェーダーからジオメトリシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos  : SV_POSITION; //システム用頂点座標
	float3 normal : NORMAL;      //法線ベクトル
	float2 uv     : TEXCOORD;    //uv値
};

//ジオメトリシェーダーからピクセルシェーダーへ
struct GSOutput
{
    float4 svpos  : SV_POSITION; //システム用頂点座標
    float3 normal : NORMAL;      //法線ベクトル
    float2 uv     : TEXCOORD;    //uv値
};

//ピクセルシェーダー出力構造体
struct PixelOutput
{
    float4 pixel_color	: SV_TARGET0;
    float4 normal	: SV_TARGET1;
};