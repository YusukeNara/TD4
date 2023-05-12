#include "Raki_DX12B.h"
#include "Raki_imguiMgr.h"
#include "RenderTargetManager.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

bool Raki_DX12B::InitDXGIDevice()
{
    HRESULT result;

#ifdef _DEBUG
	//デバッグレイヤーをオンに	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);	// 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);	// アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description;	// アダプター名

		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];	// 採用
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		// デバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(result))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;

}

bool Raki_DX12B::CreateCommand()
{
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// コマンドリストを生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

bool Raki_DX12B::CreateSwapChain()
{
	//HRESULT result = S_FALSE;

	//// 各種設定をしてスワップチェーンを生成
	//DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	//swapchainDesc.Width  = Raki_WinAPI::window_width;
	//swapchainDesc.Height = Raki_WinAPI::window_height;
	//swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 色情報の書式を一般的なものに
	//swapchainDesc.SampleDesc.Count = 1;                 // マルチサンプルしない
	//swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// バックバッファとして使えるように
	//swapchainDesc.BufferCount = 2;	                    // バッファ数を２つに設定
	//swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // フリップ後は速やかに破棄
	//swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フルスクリーン切り替えを許可
	//ComPtr<IDXGISwapChain1> swapchain1;
	//HWND hwnd = winApp->GetHWND();
	//result = dxgiFactory->CreateSwapChainForHwnd(
	//	commandQueue.Get(),
	//	winApp->GetHWND(),
	//	&swapchainDesc,
	//	nullptr,
	//	nullptr,
	//	&swapchain1);
	//if (FAILED(result)) {
	//	assert(0);
	//	return result;
	//}
	//swapchain1.Get()->Present(0, 0);
	//swapchain1.As(&swapchain);

	return true;
}

bool Raki_DX12B::CreateRenderTargetView()
{
	//HRESULT result = S_FALSE;

	//DXGI_SWAP_CHAIN_DESC swcDesc = {};
	//result = swapchain->GetDesc(&swcDesc);
	//if (FAILED(result)) {
	//	assert(0);
	//	return result;
	//}

	//// 各種設定をしてディスクリプタヒープを生成
	//D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	//heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー
	//heapDesc.NumDescriptors = swcDesc.BufferCount;
	//result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//if (FAILED(result)) {
	//	assert(0);
	//	return result;
	//}

	//// 裏表の２つ分について
	//backBuffers.resize(swcDesc.BufferCount);
	//for (int i = 0; i < backBuffers.size(); i++)
	//{
	//	// スワップチェーンからバッファを取得
	//	result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
	//	if (FAILED(result)) {
	//		assert(0);
	//		return result;
	//	}

	//	// ディスクリプタヒープのハンドルを取得
	//	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//	// レンダーターゲットビューの生成
	//	device->CreateRenderTargetView(
	//		backBuffers[i].Get(),
	//		nullptr,
	//		handle);
	//}

	return true;
}

bool Raki_DX12B::CreateSecondRenderTargetAndResource()
{
	//マルチパス結果描画用板ポリ
#pragma region mpVertex
	///ペラポリゴンリソースを作成
	mpVertex vertices[] = {
		{{-1,-1,0},{0,1}},
		{{-1, 1,0},{0,0}},
		{{ 1,-1,0},{1,1}},
		{{ 1, 1,0},{1,0}},
	};
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
	//頂点バッファー作成
	HRESULT result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mpVertBuff)
	);
	//頂点バッファビュー作成
	mpvbView.BufferLocation = mpVertBuff->GetGPUVirtualAddress();
	mpvbView.SizeInBytes = sizeof(vertices);
	mpvbView.StrideInBytes = sizeof(mpVertex);
	//データ転送
	mpVertex *vertMap = nullptr;
	result = mpVertBuff->Map(0, nullptr, (void **)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	mpVertBuff->Unmap(0, nullptr);

#pragma endregion mpVertex

	//マルチパス結果描画ポリゴン用のグラフィックスパイプラインとルートシグネチャ生成
#pragma region mpGraphicspipeline

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpstate = {};

	//シェーダーコンパイル
	ComPtr<ID3DBlob> vsblob;
	ComPtr<ID3DBlob> psblob;
	ID3DBlob *errorblob = nullptr;

	result = D3DCompileFromFile(
		L"Resources/Shaders/mpVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", 0, 0,
		vsblob.ReleaseAndGetAddressOf(),
		&errorblob
	);
	//シェーダーのエラー内容を表示
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorblob->GetBufferSize());

		std::copy_n((char *)errorblob->GetBufferPointer(),
			errorblob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = D3DCompileFromFile(
		L"Resources/Shaders/mpPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", 0, 0,
		psblob.ReleaseAndGetAddressOf(),
		&errorblob
	);
	//シェーダーのエラー内容を表示
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorblob->GetBufferSize());

		std::copy_n((char *)errorblob->GetBufferPointer(),
			errorblob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	gpstate.VS = CD3DX12_SHADER_BYTECODE(vsblob.Get());
	gpstate.PS = CD3DX12_SHADER_BYTECODE(psblob.Get());

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	gpstate.InputLayout.NumElements = _countof(layout);
	gpstate.InputLayout.pInputElementDescs = layout;

	//その他
	gpstate.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpstate.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpstate.NumRenderTargets = 1;
	gpstate.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpstate.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpstate.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpstate.SampleDesc.Count = 1;
	gpstate.SampleDesc.Quality = 0;
	gpstate.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	//ディスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE range = {};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;// t
	range.BaseShaderRegister = 0;// 0
	range.NumDescriptors = 1;
	//ルートパラメータ
	D3D12_ROOT_PARAMETER rp = {};
	rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp.DescriptorTable.pDescriptorRanges = &range;
	rp.DescriptorTable.NumDescriptorRanges = 1;
	//サンプラー設定
	D3D12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0);//s0
	//ルートシグネチャ生成
	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.pParameters = &rp;
	rsDesc.NumParameters = 1;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.pStaticSamplers = &sampler;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ComPtr<ID3DBlob> rsblob;
	result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		rsblob.ReleaseAndGetAddressOf(),
		&errorblob
	);
	device->CreateRootSignature(0, rsblob->GetBufferPointer(), rsblob->GetBufferSize(), IID_PPV_ARGS(&mpRootsig));

	//パイプラインステート生成
	gpstate.pRootSignature = mpRootsig.Get();
	result = device->CreateGraphicsPipelineState(
		&gpstate,
		IID_PPV_ARGS(&mpPipeline)
	);

	if (FAILED(result)) {
		assert(0);
	}

#pragma endregion mpGraphicspipeline

	//作成済みのヒープ情報からもう一枚作成
	auto heapDesc = rtvHeaps.Get()->GetDesc();
	//バックバッファの情報を利用
	auto &bbuff = backBuffers[0];
	auto resDesc = bbuff->GetDesc();

	heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//クリアカラー（既存のものと同じ値）
	float clearColor[] = { clearColor_r,clearColor_g, clearColor_b,clearColor_a };
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	//リソース生成
	result = device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(mpResource.ReleaseAndGetAddressOf())
	);
	//失敗時終了
	if (FAILED(result)) { return false; }

	///ビューを作成（RTV,SRV）

	//RTV用ヒープ作成
	heapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(mpRtvHeap.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) { return false; }

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension	= D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	//RTV作成
	device->CreateRenderTargetView(
		mpResource.Get(),
		&rtvDesc,
		mpRtvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	//SRV用ヒープ作成
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(mpSrvHeap.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) { return false; }

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//SRV作成
	device->CreateShaderResourceView(
		mpResource.Get(),
		&srvDesc,
		mpSrvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

bool Raki_DX12B::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Raki_WinAPI::window_width,
		Raki_WinAPI::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	//なぜかヘルパー構造体の一時オブジェクトを参照渡しするとエラー吐くようになったので
	//使用するタイミングでヘルパー構造体のオブジェクトを作成して渡すように修正
	const CD3DX12_HEAP_PROPERTIES HEAP_PROP(D3D12_HEAP_TYPE_DEFAULT);
	const CD3DX12_CLEAR_VALUE CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	// リソースの生成
	result = device->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CLEAR_VALUE,
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool Raki_DX12B::CreateFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

Raki_DX12B::~Raki_DX12B()
{
#ifdef _DEBUG
	ID3D12DebugDevice* debugDevice;
	if (SUCCEEDED(device.Get()->QueryInterface(&debugDevice)))
	{
		debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugDevice->Release();
	}

#endif
}

void Raki_DX12B::Initialize(Raki_WinAPI *win, bool isStopifFatalErrorDetected)
{
	// nullptrチェック
	assert(win);

	winApp = win;

	// DXGIデバイス初期化
	if (!InitDXGIDevice()) {
		assert(0);
	}

	// コマンド関連初期化
	if (!CreateCommand()) {
		assert(0);
	}

	// フェンス生成
	if (!CreateFence()) {
		assert(0);
	}

	//キー入力系
	if (!InitInput(win)) {
		assert(0);
	}

	//imguiデスクリプタヒープ生成
	if (!CreateDsvHeapForIngui()) {
		assert(0);
	}

	//レンダーターゲットマネージャー初期化
	RenderTargetManager::GetInstance()->InitRenderTargetManager(device.Get(), commandList.Get());

#ifdef _DEBUG

	//致命的エラー検出時、即座に停止する設定
	ID3D12InfoQueue* infoqueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoqueue)))) {
		if (isStopifFatalErrorDetected) {
			infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		}
	}

#endif // _DEBUG


}

void Raki_DX12B::StartDraw()
{

}

void Raki_DX12B::EndDraw()
{


}

void Raki_DX12B::StartDraw2()
{


}

void Raki_DX12B::StartDrawBackbuffer()
{
	////imgui描画開始
	//ImGui_ImplDX12_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

	////1パス用リソースをシェーダーリソースに戻す
	//auto changeState = CD3DX12_RESOURCE_BARRIER::Transition(
	//	mpResource.Get(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET,
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	//);
	//commandList->ResourceBarrier(1, &changeState);

	//// バックバッファの番号を取得（2つなので0番か1番）
	//UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//auto temp = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), 
	//	D3D12_RESOURCE_STATE_PRESENT, 
	//	D3D12_RESOURCE_STATE_RENDER_TARGET);

	//commandList->ResourceBarrier(1, &temp);

	//// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
	//	rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
	//	bbIndex,
	//	device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	//);
	//// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	//CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	////バックバッファをレンダーターゲットにセット
	//commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ClearDepthBuffer();
	////クリア
	//ClearRenderTarget();
}

void Raki_DX12B::StartDrawRenderTarget()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットのリソースバリアを変更
	auto changeState = CD3DX12_RESOURCE_BARRIER::Transition(
		mpResource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	commandList->ResourceBarrier(1, &changeState);
	//描画終了時にシェーダーリソースに戻す

	//1パス目レンダーターゲットディスクリプタヒープのハンドル
	auto rtvH1 = mpRtvHeap->GetCPUDescriptorHandleForHeapStart();

	// 深度バッファクリア
	//ClearDepthBuffer();

	//1パス目レンダーターゲットセット
	commandList->OMSetRenderTargets(1, &rtvH1, false, &dsvH);

	// 1パス目クリア
	float clearColor[] = { clearColor_r,clearColor_g,clearColor_b,clearColor_a };
	commandList->ClearRenderTargetView(rtvH1, clearColor, 0, nullptr);

	//上と同じエラーに対処
	auto viewport_temp = CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)Raki_WinAPI::window_width, (FLOAT)Raki_WinAPI::window_height);
	auto rect_temp = CD3DX12_RECT(0, 0, (LONG)Raki_WinAPI::window_width, (LONG)Raki_WinAPI::window_height);
	// ビューポートの設定
	commandList->RSSetViewports(1, &viewport_temp);
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &rect_temp);
}

void Raki_DX12B::CloseDraw(IDXGISwapChain4* sc)
{
	int i = 0;

	//命令クローズ
	commandList->Close();

	// コマンドリストの実行
	ID3D12CommandList *cmdLists[] = { commandList.Get() }; // コマンドリストの配列
	commandQueue->ExecuteCommandLists(1, cmdLists);

	sc->Present(1, 0);

	// コマンドリストの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	commandAllocator->Reset(); // キューをクリア
	commandList->Reset(commandAllocator.Get(), nullptr);	// 再びコマンドリストを貯める準備
}

void Raki_DX12B::EndDraw2()
{
}

void Raki_DX12B::ClearRenderTarget()
{

}

void Raki_DX12B::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

bool Raki_DX12B::InitInput(Raki_WinAPI *win)
{
	//キー入力初期化
	return Input::Get()->Init(win->GetWNDCLASSEX(), win->GetHWND());
}

bool Raki_DX12B::CreateDsvHeapForIngui()
{
	//初期化はimgui管理部が実行
	return true;
}
