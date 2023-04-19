Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
};

//スプライトの入力は1頂点のみにする

//スプライト出力構造体
struct VSOutput
{
	//頂点情報
	float4 svpos : SV_POSITION;	//頂点座標
	float2 uv    : TEXCOORD;	//UV座標
	//インスタンス情報
    matrix ins_matrix : INSTANCE_WORLD_MAT;	//インスタンシング用変換行列
    float2 size       : INSTANCE_DRAWSIZE;
    float4 uvOffset   : INSTANCE_UVOFFSET;
    float4 color      : INSTANCE_COLOR;
    float4 freedata   : INSTANCE_FREEDATA;
};

//ジオメトリ出力構造体
struct GSOutput
{
    float4 pos : SV_POSITION;	//出力頂点座標
    float2 uv  : TEXCOORD;		//出力UV値
    matrix ins_matrix : INSTANCE_WORLD_MAT;
    float2 size       : INSTANCE_DRAWSIZE;
    float4 uvOffset : INSTANCE_UVOFFSET;
    float4 color : INSTANCE_COLOR;
    float4 freedata : INSTANCE_FREEDATA;
};