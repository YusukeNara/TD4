cbuffer cbuff0 : register(b0)
{
    matrix mat;                 //ビュープロジェクション行列
    matrix matBill;             //ビルボード行列
};

struct VSOutput
{
    float4 pos   : POSITION;    //頂点座標
    float scale  : TEXCOORD;    //スケーリング
    float4 color : COLOR;       //色情報
    float rot    : ROTATE;
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //システム頂点
    float2 uv    : TEXCOORD;    //uv値
    float4 color : COLOR;       //色情報
};

struct PSOutput
{
    float4 pixel_color  : SV_TARGET0;
    float4 normal       : SV_TARGET1;
    float4 worldPos     : SV_TARGET2;
};