#include "LIT_OBJHeader.hlsli"

//	2パス目出力用シェーダー（予定）
//	現時点では、GBuffer画像を出力するだけ

//アルベドテクスチャ
Texture2D<float4> albedoTex : register(t0);
//法線テクスチャ
Texture2D<float4> normalTex : register(t1);
//ワールド座標テクスチャ
Texture2D<float4> worldTex  : register(t2);
//深度情報テクスチャ
Texture2D<float4> LdepthTex : register(t3);
//サンプラーは変更なし
SamplerState smp : register(s0);

//ピクセルのz値をライト空間の座標に変換する

float4 main(VSOutput input) : SV_TARGET
{
    //アルベド情報を取得
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //法線情報取得
    float3 normal = normalTex.Sample(smp, input.uv).xyz;
    //ワールド座標取得
    float3 worldPos = worldTex.Sample(smp, input.uv).xyz;
    //深度情報取得
    float4 depth = LdepthTex.Sample(smp, input.uv);
    
    //法線情報を復元
    normal = (normal * 2.0f) - 1.0f;

    float3 color = float3(1.0f, 1.0f, 1.0);
    
    
    //拡散反射を計算
    float3 lig = 0.0f;
    float t = max(0.0f, dot(normal, dirLight) * -1.0f);
    lig = color * t;
    
    //スペキュラ
    float3 toEye = normalize(eyePos - worldPos);
    float3 r = reflect(dirLight, normal);
    t = max(0.0f, dot(toEye, r));
    t = pow(t, 5.0f);
    lig += color * t;
    
    float4 resultColor = albedo;
    resultColor.xyz *= lig;

    return resultColor;
}