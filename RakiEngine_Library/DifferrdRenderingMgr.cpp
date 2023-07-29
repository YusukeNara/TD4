#include "DifferrdRenderingMgr.h"
#include "NY_Camera.h"
#include "RenderTargetManager.h"
#include "DirectionalLight.h"

#include "Raki_imguiMgr.h"

void DiferredRenderingMgr::Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd)
{
	m_dev = dev;
	m_cmd = cmd;

    //DirectionalLight::SetLightPos(RVector3(500.f, 1000.f, 500.f),
        //RVector3(0.f, 0.f, 0.f),
        //RVector3(0.f, 1.f, 0.f));
     
    DirectionalLight::SetLightDir(0.0f, -1.0f, 1.0f);

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            directionalLights[i].SetLightDirection(0, -1, 1, 1);
            directionalLights[i].SetLightUseFlag(true);
        }
        else {
            directionalLights[i].SetLightDirection(1, -1, 1, 0.25);
            directionalLights[i].SetLightUseFlag(true);
        }
    }
    directionalLights[0].SetLightDirection(0, -1, 1, 1.0f);
    directionalLights[0].SetLightUseFlag(true);
    directionalLights[0].SetLightUseSpecular(true);
    directionalLights[1].SetLightDirection(0, 0, 1, 0.5f);
    directionalLights[1].SetLightUseFlag(true);
    directionalLights[1].SetLightUseSpecular(false);
    directionalLights[2].SetLightDirection(-1, 0, 0, 0.5f);
    directionalLights[2].SetLightUseFlag(true);
    directionalLights[2].SetLightUseSpecular(false);
    directionalLights[3].SetLightDirection(1, 0, 0, 0.5f);
    directionalLights[3].SetLightUseFlag(true);
    directionalLights[3].SetLightUseSpecular(false);

	ShaderCompile();

	CreateGraphicsPipeline();
}

void DiferredRenderingMgr::Rendering(RTex* gBuffer, RTex* shadowMap)
{
    UpdateConstBuff();

    //ディファードレンダリング実行

    //グラフィックスパイプラインをセット
    m_cmd->SetPipelineState(m_pipelineState.Get());
    //ルートシグネチャをセット
    m_cmd->SetGraphicsRootSignature(m_rootSignature.Get());
    //プリミティブ形状設定
    m_cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //デスクリプタヒープ設定
    ID3D12DescriptorHeap* ppHeaps[] = { gBuffer->GetDescriptorHeapSRV() };
    m_cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //頂点バッファ設定
    m_cmd->IASetVertexBuffers(0, 1, &m_vbview);
    //SRVセット（計算するパラメータが増えると、ここも増える）
    m_cmd->SetGraphicsRootDescriptorTable(2,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(), 
            0, 
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(3,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            1,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(4,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            2,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    m_cmd->SetGraphicsRootDescriptorTable(5,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(gBuffer->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
            3,
            m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //定数バッファ設定（パラメーターを増やすたびにここを確認せよ）
    m_cmd->SetGraphicsRootConstantBufferView(0, m_constBuffEyePos->GetGPUVirtualAddress());
    m_cmd->SetGraphicsRootConstantBufferView(1, m_constBuffDirLight->GetGPUVirtualAddress());

    //ディファードレンダリング結果出力
    m_cmd->DrawInstanced(6, 1, 0, 0);

    //shadowMap->ClearRenderTarget();

   

    //半透明用にデプスはgBufferに、描画先をバックバッファにする
    RenderTargetManager::GetInstance()->SetDSV(gBuffer);
}

void DiferredRenderingMgr::ShowImGui()
{


}

void DiferredRenderingMgr::ShaderCompile()
{
    ComPtr<ID3DBlob> errorBlob = nullptr; //エラーオブジェクト
    HRESULT result;

//頂点シェーダーの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"Resources/Shaders/LIT_OBJVertexShader.hlsl", //シェーダーファイル名
        nullptr,//シェーダーマクロオブジェクト（今回は使わない）
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルードオブジェクト（インクルード可能にする）
        "main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
        0,
        &m_litVS, &errorBlob
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
        L"Resources/Shaders/LIT_OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &m_litPS, &errorBlob
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

}

void DiferredRenderingMgr::CreateGraphicsPipeline()
{
#pragma region VERTEX_INIT

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc  = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 6);

    HRESULT result = m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertBuff)
    );

    VertexPosUv vertex[6]{
        {{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},//左下
        {{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},//左上
        {{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},//右下
        {{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
        {{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
        {{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},//右上
        
    };

    VertexPosUv* vertMap = nullptr;
    result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
    if (SUCCEEDED(result)) {
        memcpy(vertMap, vertex, sizeof(vertex));
        m_vertBuff->Unmap(0, nullptr);
    }

    m_vbview.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
    m_vbview.SizeInBytes    = sizeof(VertexPosUv) * 6;
    m_vbview.StrideInBytes  = sizeof(VertexPosUv);

    //定数バッファ生成
    auto cbuffResdDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbuffer_b0) + 0xff) & ~0xff);
    m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &cbuffResdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constBuffEyePos)
    );

    //定数バッファデータ転送
    cbuffer_b0 *ConstMapB0 = nullptr;
    result = m_constBuffEyePos->Map(0, nullptr, (void**)&ConstMapB0);
    if (SUCCEEDED(result)) {
        ConstMapB0->eyePos = camera->GetEye();
        m_constBuffEyePos->Unmap(0, nullptr);
    }

    cbuffResdDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbuffer_b1) + 0xff) & ~0xff);
    m_dev->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &cbuffResdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constBuffDirLight)
    );

    //定数バッファデータ転送
    cbuffer_b1* ConstMapB1 = nullptr;
    result = m_constBuffDirLight->Map(0, nullptr, (void**)&ConstMapB1);
    if (SUCCEEDED(result)) {
        for (int i = 0; i < 4; i++) {
            ConstMapB1->lightData[i].dir = directionalLights[i].GetLightDirection();
            if (directionalLights[i].GetLightUseFlag()) { ConstMapB1->lightData[i].useFlag = 1; }
            else { ConstMapB1->lightData[i].useFlag = 0; }

            if (directionalLights[i].GetLightUseSpecular()) { ConstMapB1->lightData[i].useSpecular = 1; }
            else { ConstMapB1->lightData[i].useSpecular = 0; }
        }
        m_constBuffDirLight->Unmap(0, nullptr);
    }

#pragma endregion VERTEX_INIT

    //-----頂点レイアウト-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz座標
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv座標
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};

    //頂点シェーダー、ピクセルシェーダーをパイプラインに設定
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(m_litVS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(m_litPS.Get());

    //サンプルマスクとラスタライザステートの設定
    gpipeline.SampleMask        = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
    gpipeline.RasterizerState   = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //ブレンドステートの設定
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddescを書き換えるとRenderTarget[0]が書き換わる
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

    //ブレンドステートの共通設定
    blenddesc.BlendEnable       = true;//ブレンド有効
    blenddesc.BlendOpAlpha      = D3D12_BLEND_OP_ADD;//加算合成
    blenddesc.SrcBlendAlpha     = D3D12_BLEND_ONE;//ソースの値を100%使用
    blenddesc.DestBlendAlpha    = D3D12_BLEND_ZERO;//デストの値を0%使用

    //合成設定(各項目を書き換えることで設定可能)
    blenddesc.BlendOp           = D3D12_BLEND_OP_ADD;//加算
    blenddesc.SrcBlend          = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
    blenddesc.DestBlend         = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用

    //デプスステンシルステート設定
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat         = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs    = inputLayout;
    gpipeline.InputLayout.NumElements           = _countof(inputLayout);

    //図形の形状を三角形に設定
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //その他
    gpipeline.NumRenderTargets      = 1;//描画対象は1つ
    gpipeline.RTVFormats[0]         = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255指定のRGBA
    gpipeline.SampleDesc.Count      = 1;//1pxにつき1回サンプリング

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_0{};
    descRangeSRV_0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_1{};
    descRangeSRV_1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_2{};
    descRangeSRV_2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
    CD3DX12_DESCRIPTOR_RANGE descRangeSRV_3{};
    descRangeSRV_3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);


    //ルートパラメーターの設定
    CD3DX12_ROOT_PARAMETER rootparams[6] = {};
    rootparams[0].InitAsConstantBufferView(0);//b0 スペキュラ用視点座標
    rootparams[1].InitAsConstantBufferView(1);
    //GBufferテクスチャ用（定数バッファをライト配列を入れるのに使う予定だが、現状はなし）
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV_0, D3D12_SHADER_VISIBILITY_ALL);//アルベドテクスチャ
    rootparams[3].InitAsDescriptorTable(1, &descRangeSRV_1, D3D12_SHADER_VISIBILITY_ALL);//法線テクスチャ
    rootparams[4].InitAsDescriptorTable(1, &descRangeSRV_2, D3D12_SHADER_VISIBILITY_ALL);//ワールド座標テクスチャ
    rootparams[5].InitAsDescriptorTable(1, &descRangeSRV_3, D3D12_SHADER_VISIBILITY_ALL);//深度情報テクスチャ
    //定数バッファ


    //テクスチャサンプラー設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc   = {};
    samplerDesc.AddressU                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW                    = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor                 = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter                      = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD                      = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD                      = 0.0f;
    samplerDesc.ComparisonFunc              = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility            = D3D12_SHADER_VISIBILITY_PIXEL;

    //ルートシグネチャの生成
    ComPtr<ID3DBlob> errorBlob          = nullptr; //エラーオブジェクト
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc = {};
    rootsignatureDesc.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters       = rootparams;//ルートパラメーターの先頭アドレス
    rootsignatureDesc.NumParameters     = _countof(rootparams);//ルートパラメータ数
    rootsignatureDesc.pStaticSamplers   = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob        = nullptr;

    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = m_dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

    //パイプラインにルートシグネチャをセット
    gpipeline.pRootSignature = m_rootSignature.Get();

    //パイプラインステート生成
    result = m_dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pipelineState));

}

void DiferredRenderingMgr::UpdateConstBuff()
{
    //定数バッファデータ転送
    cbuffer_b0* ConstMapB0 = nullptr;
    HRESULT result = m_constBuffEyePos->Map(0, nullptr, (void**)&ConstMapB0);
    if (SUCCEEDED(result)) {
        ConstMapB0->eyePos = camera->GetEye();
        m_constBuffEyePos->Unmap(0, nullptr);
    }

    //定数バッファデータ転送
    cbuffer_b1* ConstMapB1 = nullptr;
    result = m_constBuffDirLight->Map(0, nullptr, (void**)&ConstMapB1);
    if (SUCCEEDED(result)) {
        for (int i = 0; i < 4; i++) {
            ConstMapB1->lightData[i].dir = directionalLights[i].GetLightDirection();
            if (directionalLights[i].GetLightUseFlag()) { ConstMapB1->lightData[i].useFlag = 1; }
            else { ConstMapB1->lightData[i].useFlag = 0; }
            if (directionalLights[i].GetLightUseSpecular()) { ConstMapB1->lightData[i].useSpecular = 1; }
            else { ConstMapB1->lightData[i].useSpecular = 0; }
        }
        m_constBuffDirLight->Unmap(0, nullptr);
    }
}
