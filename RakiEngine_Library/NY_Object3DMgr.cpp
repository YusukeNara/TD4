#include "NY_Object3DMgr.h"

#include "FbxLoader.h"
#include "Raki_WinAPI.h"
#include "Raki_DX12B.h"
#include "TexManager.h"
#include "RenderTargetManager.h"
#include "NY_Model.h"

ID3D12Device *NY_Object3DManager::dev = nullptr;

NY_Object3DManager::~NY_Object3DManager()
{
}

bool NY_Object3DManager::CreateObject3DManager()
{
    //デバイスのポインタを格納
    this->dev = RAKI_DX12B_DEV;

    //Object3D用パイプライン生成
    //object3dPipelineSet = Create3DPipelineState(RAKI_DX12B_DEV);

    //デイファードレンダリング用パイプラインを生成
    m_diferredRenderingPipeline = CreateDiferredRenderingPipelineState();

    m_fbxPipeline = CreateFbxPipeline();

    m_shadowMapPipeline = CreateShadowMapPipeline();

    //FBX関連
    FbxLoader::GetInstance()->Initialize();

    return true;
}

void NY_Object3DManager::FinalizeObject3DManager()
{
    FbxLoader::GetInstance()->Finalize();
}

HRESULT NY_Object3DManager::QuickDrawShadersCompile()
{
    HRESULT result;//リザルト
    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //標準ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"QuickDrawBoxGeometry.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &qd_BoxGS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);

        return result;
    }


    //ここまでシェーダーのコンパイル作業を実行
    //問題がなければ、ここでS_OKが帰る
    return result;
}

Pipeline3D NY_Object3DManager::Create3DPipelineState(ID3D12Device *dev)
{
    HRESULT result;

    //-----シェーダーの読み込みとコンパイル-----//
#pragma region LoadShader
    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJVertexShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //標準ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJGeometryShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_GS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //ピクセルシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &obj_PS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
#pragma endregion LoadShader

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//法線
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(obj_VS.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(obj_GS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(obj_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //その他
    gpipeline.NumRenderTargets = 1;//描画対象は1つ
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ルートパラメーターの設定
    //定数用
    CD3DX12_ROOT_PARAMETER rootparams[4] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準
    //ライト
    rootparams[3].InitAsConstantBufferView(3);

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    gpipelineStateDesc = gpipeline;

    //マルチパス用
    D3D12_GRAPHICS_PIPELINE_STATE_DESC mpGP{};
    mpGP = gpipelineStateDesc;

    //実験が終わったら別クラスに移動させる
    //ディファード用にピクセルシェーダー変更
    //現状抱えている問題は、このシェーダーが原因ではない（問題発生後に追加したので）


    //ブレンド設定のみ書き換える
    D3D12_RENDER_TARGET_BLEND_DESC& mpblenddesc = mpGP.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    //ブレンドステートの共通設定
    mpblenddesc.BlendEnable = true;//ブレンド有効
    mpblenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    mpblenddesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    mpblenddesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    mpblenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    mpblenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使用
    mpblenddesc.DestBlend = D3D12_BLEND_ZERO;//デストの値を100%使用


    mpGP.pRootSignature = pipelineset.rootsignature.Get();
    result = dev->CreateGraphicsPipelineState(&mpGP, IID_PPV_ARGS(&mpTexPipelineSet.pipelinestate));
    mpTexPipelineSet.rootsignature = pipelineset.rootsignature.Get();

    return pipelineset;
}

Pipeline3D NY_Object3DManager::CreateMPPipelineState(Pipeline3D defaultPP)
{
    Pipeline3D resultP;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC mpGP{};
    mpGP = gpipelineStateDesc;

    //ブレンド設定のみ書き換える
    D3D12_RENDER_TARGET_BLEND_DESC& mpblenddesc = mpGP.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    //ブレンドステートの共通設定
    mpblenddesc.BlendEnable = true;//ブレンド有効
    mpblenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    mpblenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    mpblenddesc.DestBlendAlpha = D3D12_BLEND_ONE;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    mpblenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    mpblenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使用
    mpblenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    mpGP.pRootSignature = defaultPP.rootsignature.Get();
    resultP.rootsignature = defaultPP.rootsignature;
    auto result = RAKI_DX12B_DEV->CreateGraphicsPipelineState(&mpGP, IID_PPV_ARGS(&resultP.pipelinestate));

    return resultP;
}

Pipeline3D NY_Object3DManager::CreateDiferredRenderingPipelineState()
{
    HRESULT result;

    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJVertexShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //標準ジオメトリシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJGeometryShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &obj_GS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //ピクセルシェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &obj_PS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//法線
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };



    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(obj_VS.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(obj_GS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(obj_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
    //他レンダーターゲット1にも同じ設定
    gpipeline.BlendState.RenderTarget[1] = blenddesc;
    gpipeline.BlendState.RenderTarget[2] = blenddesc;
    gpipeline.BlendState.RenderTarget[3] = blenddesc;

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //レンダーターゲット設定

    //レンダーターゲットにするGBufferを作成
    float clearcolors[4] = { 0.0f,0.0f,0.0f,0.0f };
    //レンダーターゲット設定
    //上から
    //basecolor
    //normal(法線)
    //world(ワールド座標)
    //ambient
    //diffuse
    //specular
    RenderTextureOption option[4] = {
        {DXGI_FORMAT_R8G8B8A8_UNORM,*clearcolors},
        {DXGI_FORMAT_R8G8B8A8_UNORM,*clearcolors},
        {DXGI_FORMAT_R32G32B32A32_FLOAT,*clearcolors},
        {DXGI_FORMAT_R8G8B8A8_UNORM,*clearcolors},
    };
    m_gBuffer.CreateRTex(Raki_WinAPI::window_width, Raki_WinAPI::window_height,
        clearcolors, 4, option);
    gpipeline.NumRenderTargets = 4;//描画するパラメータが増えるとここも増える
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    gpipeline.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ルートパラメーターの設定
    //定数用
    CD3DX12_ROOT_PARAMETER rootparams[4] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準
    rootparams[3].InitAsConstantBufferView(3);

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    return pipelineset;

}

Pipeline3D NY_Object3DManager::CreateFbxPipeline()
{
    HRESULT result;

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//法線
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "INDEX",0,DXGI_FORMAT_R32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };



    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定

    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト

    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/FBXVertexShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &fbx_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(fbx_VS.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(obj_GS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(obj_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
    //他レンダーターゲット1にも同じ設定
    gpipeline.BlendState.RenderTarget[1] = blenddesc;
    gpipeline.BlendState.RenderTarget[2] = blenddesc;
    gpipeline.BlendState.RenderTarget[3] = blenddesc;

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //レンダーターゲット設定

    //レンダーターゲットにするGBufferを作成
    gpipeline.NumRenderTargets = 4;//描画するパラメータが増えるとここも増える
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    gpipeline.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //ルートパラメーターの設定
    //定数用
    CD3DX12_ROOT_PARAMETER rootparams[4] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準
    //fbx
    rootparams[3].InitAsConstantBufferView(3);

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    return pipelineset;
}

Pipeline3D NY_Object3DManager::CreateShadowMapPipeline()
{
    //シャドウマップ用パイプライン

    HRESULT result;

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト
    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/ShadowMapVS.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &shadow_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/ShadowMapPS.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &shadow_PS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(shadow_VS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(shadow_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //レンダーターゲット設定

    //レンダーターゲットにするGBufferを作成
    float clearcolors[4] = { 1.0f,1.0f,1.0f,1.0f };
    //レンダーターゲット設定および生成
    RenderTextureOption option[] = {DXGI_FORMAT_R32_FLOAT,*clearcolors};
    m_shadomMap.CreateRTex(Raki_WinAPI::window_width, Raki_WinAPI::window_height,
        clearcolors, 1, option);
    gpipeline.NumRenderTargets = 1;//描画するパラメータが増えるとここも増える
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;//深度用
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //シャドウ用定数バッファは
    CD3DX12_ROOT_PARAMETER rootparams[4] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準
    rootparams[3].InitAsConstantBufferView(3);

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    return pipelineset;
}

Pipeline3D NY_Object3DManager::CreateShadowMapFbxPipeline()
{
    //シャドウマップ用パイプライン

    HRESULT result;

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {
            "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----グラフィックスパイプライン設定-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト
    //頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/ShadowMapFbxVS.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &shadowFBX_VS, &errorBlob
    );
    //シェーダーのエラー内容を表示
    if (FAILED(result))
    {
        //errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //エラー内容を出力ウインドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
    //頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(shadowFBX_VS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(shadow_PS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable = true;//ブレンド有効
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //レンダーターゲット設定

    //レンダーターゲットにするGBufferを作成
    float clearcolors[4] = { 1.0f,1.0f,1.0f,1.0f };
    //レンダーターゲット設定および生成
    RenderTextureOption option = { DXGI_FORMAT_R32_FLOAT,*clearcolors };
    gpipeline.NumRenderTargets = 1;//描画するパラメータが増えるとここも増える
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;//深度用
    gpipeline.SampleDesc.Count = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //シャドウ用定数バッファは
    CD3DX12_ROOT_PARAMETER rootparams[4] = {};
    rootparams[0].InitAsConstantBufferView(0);//定数バッファ用
    rootparams[1].InitAsConstantBufferView(1);
    //テクスチャ用
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//標準
    rootparams[3].InitAsConstantBufferView(3);

    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //ルートシグネチャの生成
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //パイプラインステート
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    return pipelineset;
}


Object3d *NY_Object3DManager::CreateObject3d(Model3D *modelData)
{
    if (modelData == nullptr) {
        assert(modelData == nullptr);
    }

    //Object3dのデータを新たに作成
    Object3d *newobj = new Object3d;

    //生成したオブジェクトのデータを初期化
    newobj->InitObject3D(dev);

    //モデルデータをセット
    newobj->SetLoadedModelData(modelData);

    //生成したオブジェクトを返却
    return newobj;
}

Object3d *NY_Object3DManager::CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum)
{
    Object3d *newobj = new Object3d;

    newobj->InitObject3D(RAKI_DX12B_DEV);

    newobj->CreateModel_Tile(x_size, y_size, x_uv, y_uv, useTexNum);

    return newobj;
}

Object3d* NY_Object3DManager::CreateModel_Box(float size, float uv_x, float uv_y, UINT useTexNum)
{
    Object3d* newobj = new Object3d;

    newobj->InitObject3D(RAKI_DX12B_DEV);

    newobj->CreateModel_Box(size, uv_x, uv_y, useTexNum);

    return newobj;
}


void NY_Object3DManager::DeleteObject3d(Object3d *obj)
{
    for (int i = 0; i < objects.size(); i++) {
        //消すオブジェクトと同じオブジェクトを検出
        if (obj == objects[i]) {
            objects.erase(objects.begin() + i);
            break;
        }
    }

    objects.shrink_to_fit();

    delete obj;
    obj = nullptr;
}

void NY_Object3DManager::UpdateAllObjects()
{
    //すべてのオブジェクトを更新する
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->UpdateObject3D();
    }
}

void NY_Object3DManager::ClearObjects()
{
    

}

void NY_Object3DManager::SetCommonBeginDrawObject3D()
{
    RenderTargetManager::GetInstance()->SetMultiRenderTargets(&m_gBuffer, 4, true);
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(m_diferredRenderingPipeline.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(m_diferredRenderingPipeline.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::CloseDrawObject3D()
{
    //レンダーターゲット切り替え
    RenderTargetManager::GetInstance()->CloseMultiRenderTargets(&m_gBuffer, 3, true);
}

void NY_Object3DManager::SetCommonBeginDrawObject3DFeatRTex(int rtHandle)
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(mpTexPipelineSet.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(mpTexPipelineSet.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap* ppHeaps[] = { RenderTargetManager::GetInstance()->renderTextures[rtHandle]->GetDescriptorHeapSRV() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetRestartObject3D()
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(m_diferredRenderingPipeline.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(m_diferredRenderingPipeline.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap* ppHeaps[] = { TexManager::texDsvHeap.Get() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetCommonBeginDrawFBX()
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(m_fbxPipeline.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(m_fbxPipeline.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap* ppHeaps[] = { TexManager::texDsvHeap.Get() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetCommonBeginDrawObject3DFeatRTex(RTex* rt)
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(mpTexPipelineSet.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(mpTexPipelineSet.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap* ppHeaps[] = { rt->GetDescriptorHeapSRV() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetCommonBeginDrawObject3D2MultiPassRenderResource()
{
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap *ppHeaps[] = { RAKI_DX12B_GET->GetMuliPassSrvDescHeap() };
    Raki_DX12B::Get()->GetGCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NY_Object3DManager::SetCommonBeginDrawShadow()
{
    //RenderTargetManager::GetInstance()->CloseMultiRenderTargets(&m_gBuffer, 4);
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(m_shadowMapPipeline.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(m_shadowMapPipeline.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //レンダーターゲット変更
    RenderTargetManager::GetInstance()->SetMultiRenderTargets(&m_shadomMap, 1);
}

void NY_Object3DManager::SetCommonBeginDrawShadow_FBX()
{
    //RenderTargetManager::GetInstance()->CloseMultiRenderTargets(&m_gBuffer, 4);
    //パイプラインステートをセット
    Raki_DX12B::Get()->GetGCommandList()->SetPipelineState(m_shadowMapFbxPipeline.pipelinestate.Get());
    //ルートシグネチャをセット
    Raki_DX12B::Get()->GetGCommandList()->SetGraphicsRootSignature(m_shadowMapFbxPipeline.rootsignature.Get());
    //プリミティブ形状設定
    Raki_DX12B::Get()->GetGCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //レンダーターゲット変更
    RenderTargetManager::GetInstance()->SetMultiRenderTargets(&m_shadomMap, 1);
}

void NY_Object3DManager::ReturnShadowToDifferd()
{
    //RenderTargetManager::GetInstance()->CloseMultiRenderTargets(&m_shadomMap, 1);

    RenderTargetManager::GetInstance()->SetMultiRenderTargets(&m_gBuffer, 3);
}

void NY_Object3DManager::ShadowMapClear()
{
}

Object3d *CreateObject3d(Model3D *modelData, RVector3 pos)
{
    //返却用のポインタ変数にObject3Dを作成
    Object3d *result = NY_Object3DManager::Get()->CreateObject3d(modelData);

    //位置を設定
    result->SetAffineParamTranslate(pos);

    //生成物を返却
    return result;
}

Object3d *LoadModel_ObjFile(string modelname)
{
    //オブジェクトデータを新規作成
    Object3d *object = new Object3d;

    //オブジェクト初期化
    object->InitObject3D(RAKI_DX12B_DEV);

    //生成したオブジェクトのモデルデータをロードする
    object->LoadAndSetModelData(modelname);

    //返却
    return object;
}

Object3d* LoadModel_FBXFile(string modelname)
{
    //オブジェクトデータを新規作成
    Object3d* object = new Object3d;

    //オブジェクト初期化
    object->InitObject3D(RAKI_DX12B_DEV);

    //生成したオブジェクトのモデルデータをロードする
    object->LoadAndSetModelData_Fbx(modelname);

    //返却
    return object;
}

Object3d* SetModel_FBX(std::shared_ptr<fbxModel> fbxModelData)
{
    //オブジェクトデータを新規作成
    Object3d* object = new Object3d;

    //オブジェクト初期化
    object->InitObject3D(RAKI_DX12B_DEV);

    //生成したオブジェクトのモデルデータをセット
    object->SetLoadedModelData(fbxModelData);

    return object;
}

void DrawObject3d(Object3d *obj)
{
    //描画準備
    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
    //オブジェクト描画
    //obj->DrawModel3D(Raki_DX12B::Get()->GetGCommandList(), Raki_DX12B::Get()->GetDevice());
}
