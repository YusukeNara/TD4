#include "RenderTargetManager.h"
#include "Raki_WinAPI.h"
#include "Raki_DX12B.h"

RenderTargetManager::RenderTargetManager()
{

}

RenderTargetManager::~RenderTargetManager()
{

}

void RenderTargetManager::InitRenderTargetManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmd)
{
	//デバイスとコマンドを設定
	dev		= device;
	cmdlist = cmd;

	CreateSwapChain();

	CreateDepthBuffer();

	CreateBackBuffers();

	//ビューポートとシザー矩形設定
	auto temp = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(Raki_WinAPI::window_width), FLOAT(Raki_WinAPI::window_height));
	default_viewport = temp;
	default_rect = CD3DX12_RECT(0, 0, LONG(Raki_WinAPI::window_width), LONG(Raki_WinAPI::window_height));
}

void RenderTargetManager::FinalizeRenderTargetManager()
{
	//コンテナ消去
	renderTextures.clear();
	renderTextures.shrink_to_fit();
}

void RenderTargetManager::CrearAndStartDraw()
{
	//すべてのレンダーテクスチャをクリアするために、一度レンダーテクスチャを設定する
	//for (int i = 0; i < renderTextures.size(); i++) {
	//	SetRenderTarget(i);
	//	ClearRenderTarget(renderTextures[i]->GetDescriptorHeapRTV());
	//	ClearDepthBuffer(renderTextures[i]->GetDescriptorHeapDSV());
	//	CloseDrawRenderTexture();
	//}

	//レンダーテクスチャはユーザーが任意のタイミングでクリアする。

	//バックバッファの番号取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//レンダーターゲットに変更
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdlist->ResourceBarrier(1, &barrier);

	//デスクリプタヒープ設定
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットに設定
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//画面クリア
	ClearBackBuffer();

	//ビューポート、シザー矩形設定
	cmdlist->RSSetViewports(1, &default_viewport);
	cmdlist->RSSetScissorRects(1, &default_rect);

	isDrawing = USING_BACKBUFFER;

	//ImGuiの描画準備
	ImguiMgr::Get()->NewFrame();
}

int RenderTargetManager::CreateRenderTexture(int width, int height)
{
	//新しく作るレンダーテクスチャ
	RTex* returnData = new RTex;

	RenderTextureOption option = {DXGI_FORMAT_R8G8B8A8_UNORM,{0.f,0.f,0.f,0.f}};

	//テクスチャデータ生成
	returnData->CreateRTex(width, height, clearcolor, 1, &option);

	//レンダーテクスチャコンテナにデータ格納
	renderTextures.emplace_back().reset(returnData);
	//末尾のイテレーター取得
	std::vector<std::shared_ptr<RTex>>::iterator itr = renderTextures.end();
	itr--;
	//distanceを使用し、インデックス番号を取得
	size_t indexnum = std::distance(renderTextures.begin(), itr);

	//キャストして、生成したレンダーテクスチャのハンドルを返却
	return static_cast<int>(indexnum);
}

void RenderTargetManager::SetRenderTarget(int handle)
{
	//ハンドルのエラーに対処

	//すでにレンダーターゲットのハンドル
	if (handle == nowRenderTargetHandle && isDrawing != USING_BACKBUFFER) {
		return;
	}

	//負の値、または範囲外参照は早期リターン
	if (handle < 0 || handle > renderTextures.size()) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}

	//空のハンドルは早期リターン
	if (renderTextures[handle] == nullptr) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		SetDrawBackBuffer();
		return;
	}

	//該当テクスチャをレンダーターゲットに設定する処理

	//前のレンダーターゲットの終了処理
	if(isDrawing != USING_BACKBUFFER){ CloseDrawRenderTexture(); }

	//テクスチャのリソースステートをレンダーターゲットに変更
	auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
		renderTextures[handle]->rtdata->rtexBuff[0].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	cmdlist->ResourceBarrier(1, &barrierState);
	
	//デスクリプタヒープ設定
	D3D12_CPU_DESCRIPTOR_HANDLE rtvh = renderTextures[handle]->GetDescriptorHeapRTV()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvh = renderTextures[handle]->GetDescriptorHeapDSV()->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲット設定
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//ビューポート、シザー矩形設定
	cmdlist->RSSetViewports(1, &renderTextures[handle]->viewport);
	cmdlist->RSSetScissorRects(1, &renderTextures[handle]->rect);

	isDrawing = USING_RENDERTEXTURE;
	nowRenderTargetHandle = handle;
}

void RenderTargetManager::SetRenderTarget(RTex* rt, int handle, bool clearFlag)
{


}

void RenderTargetManager::SetMultiRenderTargets(const RTex* renderTargets,int size, bool clearFlag)
{
	//RTV用デスクリプタハンドルは複数
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvhs;
	//デプスハンドルは一つだけ
	D3D12_CPU_DESCRIPTOR_HANDLE dsvh = renderTargets->rtdata->dsvHeap.Get()->GetCPUDescriptorHandleForHeapStart();

	//リソースバリア変更
	for (auto &rt : renderTargets->rtdata->rtexBuff) {
		//テクスチャのリソースステートをレンダーターゲットに変更
		auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
			rt.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		cmdlist->ResourceBarrier(1, &barrierState);
	}

	auto rtvHandle = renderTargets->rtdata->rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < renderTargets->rtdata->rtexBuff.size(); i++) {
		rtvhs.push_back(rtvHandle);
		rtvHandle.ptr += RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//レンダーターゲット設定
	cmdlist->OMSetRenderTargets(UINT(rtvhs.size()), rtvhs.data(), true, &dsvh);

	//レンダーターゲットクリア
	if (clearFlag) {
		for (auto& r : rtvhs) {
			cmdlist->ClearRenderTargetView(r, renderTargets->clearColors.data(), 0, nullptr);
		}
		cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	//ビューとシザーの設定
	std::vector<CD3DX12_VIEWPORT> viewports;
	std::vector<CD3DX12_RECT> rects;

	for (int i = 0; i < int(rtvhs.size()); i++) {
		viewports.push_back(renderTargets->viewport);
		rects.push_back(renderTargets->rect);
	}

	cmdlist->RSSetViewports(UINT(rtvhs.size()), viewports.data());
	cmdlist->RSSetScissorRects(UINT(rtvhs.size()), rects.data());

	isDrawing = USING_RENDERTEXTURE;
}

void RenderTargetManager::SetRenderTargetForRTexSpecifiedIndex(const RTex* renderTargets, int targetIndex, bool clearFlag)
{
	//RTV用デスクリプタハンドルはRTexの目標インデックス1つだけ
	auto rtvh = renderTargets->rtdata->rtvHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < targetIndex; i++) {
		rtvh.ptr += RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//デプスハンドルは一つだけ
	D3D12_CPU_DESCRIPTOR_HANDLE dsvh = renderTargets->rtdata->dsvHeap.Get()->GetCPUDescriptorHandleForHeapStart();

	//リソースバリア変更
	auto rt = renderTargets->rtdata->rtexBuff[targetIndex];
	auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
		rt.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	cmdlist->ResourceBarrier(1, &barrierState);

	//レンダーターゲット設定
	cmdlist->OMSetRenderTargets(1, &rtvh, true, &dsvh);

	//レンダーターゲットクリア
	if (clearFlag) {
		cmdlist->ClearRenderTargetView(rtvh, renderTargets->clearColors.data(), 0, nullptr);
		cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	//ビューとシザーの設定
	std::vector<CD3DX12_VIEWPORT> viewports;
	std::vector<CD3DX12_RECT> rects;

	viewports.push_back(renderTargets->viewport);
	rects.push_back(renderTargets->rect);

	cmdlist->RSSetViewports(1, viewports.data());
	cmdlist->RSSetScissorRects(1, rects.data());

	isDrawing = USING_RENDERTEXTURE;
}

void RenderTargetManager::CloseMultiRenderTargets(const RTex* renderTargets, int size,bool isChangeBB)
{
	//レンダーターゲットをクローズ、バックバッファに切り替え
	for (auto& rt : renderTargets->rtdata->rtexBuff) {
		//テクスチャのリソースステートをレンダーターゲットに変更
		auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
			rt.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		);
		cmdlist->ResourceBarrier(1, &barrierState);
	}
	
	if (isChangeBB) {
		SetDrawBackBuffer();
	}

}

void RenderTargetManager::CloseMultiRenderTargetsForRTexSpecifiedIndex(const RTex* renderTargets, int targetIndex, bool isChangeBB)
{
	auto rt = renderTargets->rtdata->rtexBuff[targetIndex];
	//テクスチャのリソースステートをレンダーターゲットに変更
	auto barrierState = CD3DX12_RESOURCE_BARRIER::Transition(
		rt.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	cmdlist->ResourceBarrier(1, &barrierState);
	

	if (isChangeBB) {
		SetDrawBackBuffer();
	}
}

void RenderTargetManager::SetRenderTargetDrawArea(int handle, int x1, int y1, int x2, int y2)
{
	//チェック
	if (isNullHandle(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		return;
	}
	if (isHandleOutOfRange(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}

	//画像サイズより大きい場合はサイズに抑える
	int temp_sizeX = renderTextures[handle]->graph_size.first;
	int temp_sizeY = renderTextures[handle]->graph_size.second;

	if (temp_sizeX < x2 - x1) { x2 = x1 + temp_sizeX; }
	if (temp_sizeY < y2 - y1) { y2 = y1 + temp_sizeY; }

	renderTextures[handle]->rect = CD3DX12_RECT(x1, y1, x2, y2);
}

void RenderTargetManager::SetRenderTargetClipingArea(int handle, int x1, int y1, int x2, int y2)
{
	//チェック
	if (isNullHandle(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Pointing to an empty handle." << std::endl;
		return;
	}
	if (isHandleOutOfRange(handle)) {
		std::cout << "ERROR : RENDERTARGETMANAGER : Render texture handle is out of range" << std::endl;
		return;
	}

	renderTextures[handle]->viewport = CD3DX12_VIEWPORT(FLOAT(x1), FLOAT(y1), FLOAT(x2), FLOAT(y2));
}

void RenderTargetManager::SetClearColor(float red, float green, float blue)
{
	clearcolor[0] = red;
	clearcolor[1] = green;
	clearcolor[2] = blue;
}

void RenderTargetManager::ClearRenderTexture(int handle)
{
	int handletmp = 0;
	if (handle != nowRenderTargetHandle && isDrawing != USING_BACKBUFFER) {
		handletmp = nowRenderTargetHandle;
		SetRenderTarget(handle);
		ClearRenderTarget(renderTextures[handle]->GetDescriptorHeapRTV());
		ClearDepthBuffer(renderTextures[handle]->GetDescriptorHeapDSV());
		CloseDrawRenderTexture();
		
		SetRenderTarget(handletmp);
	}
	else {
		ClearRenderTarget(renderTextures[nowRenderTargetHandle]->GetDescriptorHeapRTV());
		ClearDepthBuffer(renderTextures[nowRenderTargetHandle]->GetDescriptorHeapDSV());
	}

}
void RenderTargetManager::SetDrawBackBuffer()
{
	//レンダーテクスチャの状態を表示状態に
	if (isDrawing == USING_BACKBUFFER) { return; }
	
	//CloseDrawRenderTexture();

	//バックバッファの番号取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//デスクリプタヒープ設定
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットに設定
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);

	//ビューポート、シザー矩形設定
	cmdlist->RSSetViewports(1, &default_viewport);
	cmdlist->RSSetScissorRects(1, &default_rect);

	isDrawing = USING_BACKBUFFER;
}

void RenderTargetManager::SetDSV(RTex* gBuffer)
{
	//バックバッファの番号取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	//デスクリプタヒープ設定
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(gBuffer->rtdata->dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットに設定
	cmdlist->OMSetRenderTargets(1, &rtvh, false, &dsvh);
}

void RenderTargetManager::SwapChainBufferFlip()
{
	//Imgui描画実行
	ImguiMgr::Get()->SendImguiDrawCommand();

	//バックバッファをクローズ
	CloseDrawBackBuffer();

	//描画終了コマンド
	Raki_DX12B::Get()->CloseDraw(swapchain.Get());
}

void RenderTargetManager::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = Raki_WinAPI::window_width;
	swapchainDesc.Height = Raki_WinAPI::window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 色情報の書式を一般的なものに
	swapchainDesc.SampleDesc.Count = 1;                 // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// バックバッファとして使えるように
	swapchainDesc.BufferCount = 2;	                    // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // フリップ後は速やかに破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フルスクリーン切り替えを許可
	ComPtr<IDXGISwapChain1> swapchain1;
	result = Raki_DX12B::Get()->GetDXGIFactory()->CreateSwapChainForHwnd(
		Raki_DX12B::Get()->GetCmdQueue(),
		Raki_WinAPI::GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);
	if (FAILED(result)) {
		assert(0);
	}
	swapchain1.Get()->Present(0, 0);
	swapchain1.As(&swapchain);

}

void RenderTargetManager::CreateBackBuffers()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
	}

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
	if (FAILED(result)) {
		assert(0);
	}

	// 裏表の２つ分について
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		if (FAILED(result)) {
			assert(0);
		}

		// ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			i, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}

}

void RenderTargetManager::CreateDepthBuffer()
{
	//ウィンドウサイズに合わせてバッファを生成する
	CD3DX12_RESOURCE_DESC depthresdesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			Raki_WinAPI::window_width,
			Raki_WinAPI::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto clearvalue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//深度バッファ生成
	HRESULT rtex_depthbuff_create_result = RAKI_DX12B_DEV->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearvalue,
		IID_PPV_ARGS(&depthBuff)
	);

	//生成失敗は強制終了
	assert(SUCCEEDED(rtex_depthbuff_create_result));

	//深度バッファからデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC depthheapdesc{};
	depthheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthheapdesc.NumDescriptors = 1;

	auto rtex_dsv_descheap_create_result = RAKI_DX12B_DEV->CreateDescriptorHeap(
		&depthheapdesc,
		IID_PPV_ARGS(&dsvHeap)
	);
	assert(SUCCEEDED(rtex_dsv_descheap_create_result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RAKI_DX12B_DEV->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void RenderTargetManager::ClearDepthBuffer(ID3D12DescriptorHeap* dsv)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsv->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void RenderTargetManager::ClearRenderTarget(ID3D12DescriptorHeap* rtv)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearRenderTargetView(rtvh, clearcolor, 0, nullptr);
}

void RenderTargetManager::ClearBackBuffer()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	cmdlist->ClearDepthStencilView(dsvh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvh = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		bbIndex,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	cmdlist->ClearRenderTargetView(rtvh, clearcolor, 0, nullptr);
}

void RenderTargetManager::CloseDrawBackBuffer()
{
	// バックバッファのリソースバリアを変更（描画対象→表示状態）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	auto barrier_temp = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	
	//リソースバリア
	cmdlist->ResourceBarrier(1, &barrier_temp);
}

void RenderTargetManager::CloseDrawRenderTexture()
{

	//現在のレンダーテクスチャを表示状態に
	auto resourceBattier = CD3DX12_RESOURCE_BARRIER::Transition(
		renderTextures[nowRenderTargetHandle]->GetTextureBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	RAKI_DX12B_CMD->ResourceBarrier(1, &resourceBattier);
}

void RenderTargetManager::BufferFlip()
{
	//スワップチェーンをフリップ
	swapchain->Present(1, 0);

}
