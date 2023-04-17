#include "Sprite.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    //return tex.Sample(smp, input.uv);
    
    ////簡単なぼかし処理のやり方
    //float w, h, levels;
    ////この関数でテクスチャのサイズ情報が取れる
    //tex.GetDimensions(0, w, h, levels);
    ////サイズ情報を取得するのは隣のピクセルの情報を取得するため
    ////画素を取得して、ぼかしの強さ（power）で割る
    //float power = 9;
    ////画素移動のオフセット値を算出
    //float dx = 1.0f / w;
    //float dy = 1.0f / h;
    //float4 ret = float4(0, 0, 0, 0);
    
    ////以下、画素抽出（3 * 3）
    
    ////上の画素
    ////ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)); //左上
    //ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)) * -1; //上
    ////ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy)); //右上
    ////中央の画素
    //ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)) * -1; //左
    //ret += tex.Sample(smp, input.uv) * 4; //自分
    //ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)) * -1; //右
    ////下の画素
    ////ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)); //左下
    //ret += tex.Sample(smp, input.uv + float2(0, 2 * dy)) * -1; //下
    ////ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)); //右下
    
    ////ぼかし
    ////ぼかしの強さで割って返却
    ////return ret / power;

    ////エッジ強調
    ////上記の画素のうち、上下左右中央のテーブルを使用
    ////中央を強調、上下左右を-1する
    
    ////その後反転
    //float Y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
    ////エッジを更に強調
    //Y = pow(1.0f - Y, 10.0f);
    ////余計な輪郭を消す
    //Y = step(0.2, Y);
    
    //return float4(Y, Y, Y, 1);
    
    
    //走査線
    //float Time = color.a * 40;
    
    //float2 samplePoint = input.uv;
    //float4 Tex = tex.Sample(smp, samplePoint);
    //float sinv = sin(input.uv.y * 2 + Time * -0.1);
    //float steped = step(0.99, sinv * sinv);
    //Tex.rgb -= (1 - steped) * abs(sin(input.uv.y * 50.0f + Time * 1.0)) * 0.02;
    //Tex.rgb -= (1 - steped) * abs(sin(input.uv.y * 100.0f - Time * 2.0)) * 0.06;
    //Tex.rgb += steped * 0.1;
    
    //樽状湾曲
    //float2 sp = input.uv;
    //sp -= float2(0.5, 0.5);
    //float distPower = pow(length(sp), 500);
    //sp *= float2(distPower, distPower);
    //sp += float2(0.5, 0.5);
    //float4 Tex = tex.Sample(smp, sp);
    
    //ビネット
    float2 samp = input.uv;
    float4 Tex = tex.Sample(smp, samp);
    float vignette = length(float2(0.5f, 0.5f) - input.uv);
    vignette = clamp(vignette - 0.2, 0, 1);
    Tex.rgb -= vignette;
    
    return Tex;

}