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

//並行光源の計算
float3 DirectionalShadeing(float4 lStatus, float3 normal, float3 worldPos, float3 color, uint useSpecular)
{
    float3 result;

    float t = max(0.0f, dot(normal, lStatus.xyz) * -1.0f);
    result = color * t;
    
    //スペキュラ
    if (useSpecular)
    {
        float3 toEye = normalize(eyePos - worldPos);
        float3 r = reflect(lStatus.xyz, normal);
        t = max(0.0f, dot(toEye, r));
        t = pow(t, 5.0f);
        result += color * t;
    }
    
    return result * lStatus.w;
}


float4 main(VSOutput input) : SV_TARGET
{
    //アルベド情報を取得
    float4 albedo = albedoTex.Sample(smp, input.uv);
    //法線情報取得
    float3 normal = normalTex.Sample(smp, input.uv).rgb;
    //ワールド座標取得
    float3 worldPos = worldTex.Sample(smp, input.uv).xyz;
    //深度情報取得
    float4 depth = LdepthTex.Sample(smp, input.uv);
    //スペキュラ強度取得
    float specPower = normalTex.Sample(smp, input.uv).a;
    
    //法線情報を復元
    normal = (normal * 2.0f) - 1.0f;

    float3 color = float3(1.0f, 1.0f, 1.0);
    
    float4 resultColor = float4(0, 0, 0, albedo.w);
    
    float3 lig = float3(0, 0, 0);
    
    //並行光源の計算結果
    for (int i = 0; i < 4; i++)
    {
        if (lightStatus[i].isActive)
        {
            float3 l = float3(0, 0, 0);
            float3 d = float3(0, 0, 0);
            d = DirectionalShadeing(lightStatus[i].lightDir, normal, worldPos, color, lightStatus[i].isUseSpecular);
            l = (albedo.xyz * d);
            resultColor.xyz += l;
        }
    }
    
    //拡散反射を計算
    //float3 lig = 0.0f;
    //float t = max(0.0f, dot(normal, lightStatus[0].xyz) * -1.0f);
    //lig = color * t;
    
    ////スペキュラ
    //float3 toEye = normalize(eyePos - worldPos);
    //float3 r = reflect(lightStatus[0].xyz, normal);
    //t = max(0.0f, dot(toEye, r));
    //t = pow(t, 5.0f);
    //lig += color * t;
    
    //resultColor = albedo;
    //resultColor.xyz *= lig;

    return resultColor;
}