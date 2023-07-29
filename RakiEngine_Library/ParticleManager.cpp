#include <d3dcompiler.h>

#include "ParticleManager.h"
#include "Raki_DX12B.h"
#include "TexManager.h"
#include "NY_random.h"
#include "RakiUtility.h"

#include <iostream>

#pragma comment(lib,"d3dcompiler.lib")

const int ParticleManager::MAX_VERTEX;

ParticleManager *ParticleManager::Create(bool is2d, ParticleBlendState pBlendState) {

	//パーティクルマネージャー生成
	ParticleManager *pm = new ParticleManager(
		Raki_DX12B::Get()->GetDevice(),
		Raki_DX12B::Get()->GetGCommandList()
	);

	//生成したものを初期化
	pm->Initialize(is2d, pBlendState);

	pm->is2DParticle = is2d;

	return pm;
}

void ParticleManager::Initialize(bool is2d, ParticleBlendState pBlendState) {
	//nullチェック

	HRESULT result;

	//パイプライン初期化
	InitializeGraphicsPipeline(is2d, pBlendState);

	//パーティクル用モデル作成
	CreateModel();

	//定数バッファ生成
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::Update() {

	//寿命が切れたパーティクルを削除
	grains.remove_if([](Particle &p) {return p.nowFrame >= p.endFrame; });

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator itr = grains.begin();
		itr != grains.end(); itr++) {

		//フレーム数カウント
		itr->nowFrame++;

		//進行度を0~1の割合に
		float rate = (float)itr->nowFrame / itr->endFrame;

		//速度加算
		itr->vel = itr->vel + itr->acc;

		//速度による移動
		itr->pos += itr->vel;

		//色線形補間
		itr->color = itr->s_color + (itr->e_color - itr->s_color) * rate;

		//スケーリングの線形補間
		itr->scale = itr->s_scale + (itr->e_scale - itr->s_scale) * rate;

		//回転線形補間
		itr->rot = itr->s_rotation + (itr->e_rotation - itr->s_rotation) / rate;
	}

	//頂点バッファデータ転送
	int vcount = 0;
	PVertex *vertMap = nullptr;
	result = vertbuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle>::iterator it = grains.begin();
			it != grains.end();
			it++) {
			// 座標
			vertMap->pos = it->pos;
			// スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			//回転
			vertMap->rot = it->rot;
			// 次の頂点へ
			vertMap++;
			if (++vcount >= MAX_VERTEX) {
				break;
			}
		}
		vertbuff->Unmap(0, nullptr);
	}

	//定数バッファデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (result == S_OK) {
		//ビュープロジェクション行列
		constMap->mat = camera->GetMatrixViewProjection();
		//全方向ビルボード
		constMap->matBillBoard = camera->GetMatrixBillBoardAll();
		//色
		constBuff->Unmap(0, nullptr);
	}

}

void ParticleManager::Draw(UINT drawTexNum)
{

	UINT drawNum = (UINT)std::distance(grains.begin(), grains.end());
	if (drawNum > MAX_VERTEX) {
		drawNum = MAX_VERTEX;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	// パイプラインステートの設定
	cmd->SetPipelineState(pipeline.Get());
	// ルートシグネチャの設定
	cmd->SetGraphicsRootSignature(rootsig.Get());
	// プリミティブ形状を設定
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmd->IASetVertexBuffers(0, 1, &vbview);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
	cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			drawTexNum, Raki_DX12B::Get()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// 描画コマンド
	cmd->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(ParticleGrainState pgState)
{
	//要素追加
	grains.emplace_front();
	//追加した要素の参照
	Particle &p = grains.front();

	if (pgState.isRandomSpawn) {
		p.pos = rutility::randomRV3(
			pgState.position + pgState.position_spawnRange1,
			pgState.position + pgState.position_spawnRange2);
	}
	else {
		p.pos = pgState.position;			//初期位置
	}
	p.vel = pgState.vel;			//速度
	p.acc = pgState.acc;			//加速度
	p.s_scale = pgState.scale_start; //開始時のスケールサイズ
	p.e_scale = pgState.scale_end;	//終了時のスケールサイズ
	p.endFrame = pgState.aliveTime;		//生存時間
	p.s_color = pgState.color_start;
	p.e_color = pgState.color_end;
	p.s_rotation = pgState.rot_start;
	p.e_rotation = pgState.rot_end;
}

void ParticleManager::Prototype_Set(ParticlePrototype *proto)
{
	prototype_.reset(proto);
}

void ParticleManager::Prototype_Add(int addCount, RVector3 startPos)
{
	for (int i = 0; i < addCount; i++) {
		//uniqueポインタで動的生成
		std::unique_ptr<ParticlePrototype> newp(prototype_->clone(startPos));
		pplist.emplace_front(std::move(newp));
	}
}

void ParticleManager::Prototype_Update()
{

	pplist.remove_if([](std::unique_ptr<ParticlePrototype> &p) {
		return p->nowFrame >= p->endFrame;
		});

	//バッファデータ転送
	int vcount = 0;
	PVertex *vertMap = nullptr;
	result = vertbuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<std::unique_ptr<ParticlePrototype>>::iterator it = pplist.begin();
			it != pplist.end();
			it++) {
			(*it)->nowFrame++;
			(*it)->Update();
			// 座標
			vertMap->pos = (*it)->pos;
			if (is2DParticle) {
				vertMap->pos.z = 0.0f;
			}
			// スケール
			vertMap->scale = (*it)->scale;
			//色
			vertMap->color = (*it)->color;
			if(is2DParticle){ vertMap->worldmat = XMMatrixTranslation((*it)->pos.x, (*it)->pos.y, (*it)->pos.z); }
			else { vertMap->worldmat = XMMatrixIdentity(); }
			

			// 次の頂点へ
			vertMap++;
			if (++vcount >= MAX_VERTEX) {
				break;
			}
		}
		vertbuff->Unmap(0, nullptr);
	}

	//定数バッファデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (result == S_OK) {
		//ビュープロジェクション行列
		if (is2DParticle) {
			//プロジェクション行列
			constMap->mat = camera->GetMatrixProjection2D();

			constMap->matBillBoard = XMMatrixIdentity();
		}
		else {
			constMap->mat = camera->GetMatrixViewProjection();
			//全方向ビルボード
			constMap->matBillBoard = camera->GetMatrixBillBoardAll();
		}
		//色
		constBuff->Unmap(0, nullptr);
	}

}

void ParticleManager::Prototype_Draw(UINT drawTexNum)
{
	UINT drawNum = (UINT)std::distance(pplist.begin(), pplist.end());
	if (drawNum > MAX_VERTEX) {
		drawNum = MAX_VERTEX;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	// パイプラインステートの設定
	cmd->SetPipelineState(pipeline.Get());
	// ルートシグネチャの設定
	cmd->SetGraphicsRootSignature(rootsig.Get());
	// プリミティブ形状を設定
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmd->IASetVertexBuffers(0, 1, &vbview);
	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
	cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmd->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			drawTexNum, Raki_DX12B::Get()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// 描画コマンド
	cmd->DrawInstanced(drawNum, 1, 0, 0);
}



void ParticleManager::InitializeGraphicsPipeline(bool is2d, ParticleBlendState pBlendState) {

	result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	if (is2d) {
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/Particle2dVS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/Particle2dPS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&psBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
		// ジオメトリシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/Particle2dGS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&gsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}
	else {
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&psBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// ジオメトリシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&gsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}



	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"ROTATE", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};



	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	if (is2d) {
		//デプスステンシルステート設定
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き
		gpipeline.DepthStencilState.DepthEnable = false;//深度テスト無効
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
	}
	else {
		// デプスの書き込みを禁止
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}


	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;

	switch (pBlendState)
	{
	case ParticleBlendState::PBLEND_MODE_ADD:

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case ParticleBlendState::PBLEND_MODE_MIN:

		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case ParticleBlendState::PBLEND_MODE_ALPHA:

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
		break;

	default:
		break;
	}
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// アルファブレンディング
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
	//// 減算ブレンディング


	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.RenderTarget[1] = blenddesc;
	//gpipeline.BlendState.RenderTarget[2] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（点）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は3つディファー土に合わせる
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	//gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	//gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsig));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsig.Get();

	// グラフィックスパイプラインの生成
	result = Raki_DX12B::Get()->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline));

	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::CreateModel() {
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto res = CD3DX12_RESOURCE_DESC::Buffer(sizeof(PVertex) * MAX_VERTEX);
	result = dev->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&res,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertbuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//// 頂点バッファビューの作成
	vbview.BufferLocation = vertbuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeof(PVertex) * MAX_VERTEX;
	vbview.StrideInBytes = sizeof(PVertex);


}

void ParticleDrawManager::Init()
{
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト
	
#pragma region SHADER_COMPILE
	// 頂点シェーダの読み込みとコンパイル
	HRESULT result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion SHADER_COMPILE

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"ROTATE", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	// アルファブレンディング
	//blenddesc.BlendOp		= D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend	= D3D12_BLEND_SRC_ALPHA;//ソースの値を100%使用
	//blenddesc.DestBlend	= D3D12_BLEND_INV_SRC_ALPHA;//デストの値を100%使用
	//// 加算ブレンディング
	blenddesc.BlendOp	= D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend	= D3D12_BLEND_ONE;//ソースの値を100%使用
	blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使用

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//gpipeline.BlendState.RenderTarget[1] = blenddesc;
	//gpipeline.BlendState.RenderTarget[2] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（点）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は3つディファー土に合わせる
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	//gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	//gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = RAKI_DX12B_DEV->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsig));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsig.Get();

	// グラフィックスパイプラインの生成
	result = Raki_DX12B::Get()->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline3D));

	if (FAILED(result)) {
		assert(0);
	}

}

void ParticleDrawManager::SetCommonBeginDrawParticle3D()
{
	RAKI_DX12B_CMD->SetPipelineState(pipeline3D.Get());

	RAKI_DX12B_CMD->SetGraphicsRootSignature(rootsig.Get());
}

void ParticleDrawManager::SetCommonBeginDrawParticle2D()
{
	RAKI_DX12B_CMD->SetPipelineState(pipeline2D.Get());
}
