#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <initguid.h>
#include <dxgidebug.h>
#include <cstdlib>
#include <iostream>

#include "Raki_WinAPI.h"
#include "Raki_Input.h"

//マルチパス結果描画用ポリゴン頂点データ
struct mpVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

//hresult出力
inline void ExportHRESULTmessage(HRESULT resultCode){ 
	if(FAILED(resultCode)){ std::cout << std::system_category().message(resultCode) << std::endl; }
}


class Raki_DX12B
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Raki_DX12B() {};
	~Raki_DX12B();

public:
	//--------------------メンバ関数--------------------//

	//コピーコンストラクタ、代入演算子無効化
	Raki_DX12B &operator=(const Raki_DX12B &obj) = delete;
	Raki_DX12B(const Raki_DX12B &obj) = delete;

	static Raki_DX12B *Get() {
		static Raki_DX12B dx12b;
		return &dx12b;
	}

	/// <summary>
	/// DirectX12の一括初期化
	/// </summary>
	void Initialize(Raki_WinAPI* win, bool isStopifFatalErrorDetected = false);

	/// <summary>
	/// 1パス目描画開始
	/// </summary>
	void StartDraw();

	/// <summary>
	/// 1パス目描画終了
	/// </summary>
	void EndDraw();

	/// <summary>
	/// 従来の描画開始
	/// </summary>
	void StartDraw2();

	//バックバッファ描画開始
	void StartDrawBackbuffer();

	//レンダーターゲット描画開始（ここにレンダリングした結果を画像として使える）
	void StartDrawRenderTarget();

	//描画処理終了
	void CloseDraw(IDXGISwapChain4 *sc);

	/// <summary>
	/// 従来の描画終了
	/// </summary>
	void EndDraw2();

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファーのクリア
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// ID3D12GraphicsCommandListのゲッタ
	/// </summary>
	/// <returns>ID3D12GraphicsCommandListのポインタ</returns>
	ID3D12GraphicsCommandList *GetGCommandList() { return commandList.Get(); }

	/// <summary>
	/// ID3D12Deviceのゲッタ
	/// </summary>
	/// <returns></returns>
	ID3D12Device *GetDevice() { return device.Get(); }

	IDXGIFactory6* GetDXGIFactory() { return dxgiFactory.Get(); }

	ID3D12CommandQueue* GetCmdQueue() { return commandQueue.Get(); }

	/// <summary>
	/// imgui用デスクリプタヒープのゲッタ
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> GetDsvHeapForImgui() { return dsvHeapForImgui; }

	/// <summary>
	/// マルチパス用ディスクリプタヒープ
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap *GetMuliPassSrvDescHeap() { return mpSrvHeap.Get(); }

	IDXGISwapChain4* GetSwapChain() { return swapchain.Get(); }


	void ManualRelease() {
		dxgiFactory.ReleaseAndGetAddressOf();
		device.ReleaseAndGetAddressOf();
		commandAllocator.ReleaseAndGetAddressOf();
		commandList.ReleaseAndGetAddressOf();
		commandQueue.ReleaseAndGetAddressOf();
		depthBuffer.ReleaseAndGetAddressOf();
		rtvHeaps.ReleaseAndGetAddressOf();
		dsvHeap.ReleaseAndGetAddressOf();
		fence.ReleaseAndGetAddressOf();
	}

private:
	//DirectX12要素のメンバ変数

	// ウィンドウズアプリケーション管理
	Raki_WinAPI *winApp = nullptr;

	// Direct3D関連
	ComPtr<IDXGIFactory6>				dxgiFactory;
	ComPtr<ID3D12Device>				device;
	ComPtr<ID3D12Debug1>				debugController;
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	ComPtr<ID3D12CommandAllocator>		commandAllocator;
	ComPtr<ID3D12CommandQueue>			commandQueue;
	ComPtr<IDXGISwapChain4>				swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource>				depthBuffer;
	ComPtr<ID3D12DescriptorHeap>		rtvHeaps;
	ComPtr<ID3D12DescriptorHeap>		dsvHeap;
	ComPtr<ID3D12Fence>					fence;
	UINT64								fenceVal = 0;

	//オプション

	//クリアカラー
	float clearColor_r = 0.1f;
	float clearColor_g = 0.25f;
	float clearColor_b = 0.5f;
	float clearColor_a = 0.0f;

	//imgui関連
	ComPtr<ID3D12DescriptorHeap>		dsvHeapForImgui;

	//マルチパス関連

	//リソース
	ComPtr<ID3D12Resource>				mpResource;	//ペラポリゴンリソース
	ComPtr<ID3D12DescriptorHeap>		mpRtvHeap;	//レンダーターゲット用
	ComPtr<ID3D12DescriptorHeap>		mpSrvHeap;	//テクスチャ用

	ComPtr<ID3D12Resource>				mpVertBuff; //結果描画用頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW			mpvbView;	//結果描画用頂点バッファビュー
	ComPtr<ID3D12PipelineState>			mpPipeline;	//ペラポリゴン用パイプライン
	ComPtr<ID3D12RootSignature>			mpRootsig;	//ペラポリゴン用ルートシグネチャ
	ComPtr<ID3D12Resource>				mpDepth;

	bool isRenderTarget = false;
	bool isBackBuffer = false;


private:
	//DirectX12の初期化関数群

	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitDXGIDevice();

	/// <summary>
	/// コマンド関連の生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateCommand();

	/// <summary>
	/// スワップチェーン生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateSwapChain();

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateRenderTargetView();

	bool CreateSecondRenderTargetAndResource();

	/// <summary>
	/// 深度バッファー生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateDepthBuffer();

	/// <summary>
	/// フェンス生成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateFence();

	/// <summary>
	/// 入力系初期化
	/// </summary>
	bool InitInput(Raki_WinAPI *win);

	/// <summary>
	/// imgui用デスクリプタヒープ
	/// </summary>
	/// <returns></returns>
	bool CreateDsvHeapForIngui();

};

//<<<<<<< HEAD
//
//=======
//>>>>>>> master
#define RAKI_DX12B_DEV (Raki_DX12B::Get()->GetDevice())
#define RAKI_DX12B_CMD (Raki_DX12B::Get()->GetGCommandList())
#define RAKI_DX12B_GET (Raki_DX12B::Get())
