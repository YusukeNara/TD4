#include "DebugDrawer.h"

DebugDrawer* DebugDrawer::GetInstance()
{
    static DebugDrawer ins;

    return &ins;
}

void DebugDrawer::Init()
{
    //HRESULT result;

    //シェーダーコンパイル
    ////頂点シェーダーの読み込みとコンパイル
    //ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト
    //result = D3DCompileFromFile(
    //    L"Resources/Shaders/SpriteVS.hlsl", //シェーダーファイル名
    //    nullptr,//シェーダーマクロオブジェクト（今回は使わない）
    //    D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
    //    "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
    //    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
    //    0,
    //    &line_VS, &errorBlob
    //);
    ////シェーダーのエラー内容を表示
    //if (FAILED(result))
    //{
    //    //errorBlobからエラー内容をstring型にコピー
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char*)errorBlob->GetBufferPointer(),
    //        errorBlob->GetBufferSize(),
    //        errstr.begin());
    //    errstr += "\n";
    //    //エラー内容を出力ウインドウに表示
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    //パイプライン生成










}

void DebugDrawer::Rendering()
{
}

void DebugDrawer::DrawLine(RVector3 pos1, RVector3 pos2)
{
}
