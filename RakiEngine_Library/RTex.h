#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <memory>
#include <DirectXTex.h>
#include <array>

struct RenderTextureOption
{
	DXGI_FORMAT format;
	float clearColor[4];
};

//レンダーテクスチャ一枚のデータ
class RenderTextureData
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//デフォルト
	RenderTextureData() = default;
	~RenderTextureData() = default;
/// <summary>
	/// レンダーテクスチャ初期化
/// </summary>
	/// <param name="texwidth">レンダーテクスチャ横幅</param>
	/// <param name="texheight">レンダーテクスチャ縦幅</param>
	/// <param name="clearColor">クリアカラー</param>
	/// <param name="addBufferNums">Rtex一枚に生成するバッファの数</param>
	void Init(int texwidth, int texheight, float* clearColor, int addBufferNums, RenderTextureOption option[]);

private:
	//テクスチャバッファ（vector）
	std::vector<ComPtr<ID3D12Resource>>		rtexBuff;
	//テクスチャ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>			srvHeap	  = nullptr;
	//レンダーターゲット用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>			rtvHeap	  = nullptr;
	//デプス用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>			dsvHeap   = nullptr;
	//深度バッファ
	ComPtr<ID3D12Resource>		depthBuff;

	//テクスチャバッファ生成
	void CreateTextureBuffer(int texture_width, int texture_height, float* clearColor, int addBufferNums, RenderTextureOption option[]);

	//SRVデスクリプタヒープ生成
	void CreateSRVDescriptorHeap(int bufferCount,RenderTextureOption option[]);

	//RTVデスクリプタヒープ作成
	void CreateRTVDescriptorHeap(int bufferCount);

	//深度バッファ生成（画像サイズ）
	void CreateDepthBuffer(int texture_width, int texture_height);

	//DSVデスクリプタヒープ作成
	void CreateDSVDescriptorHeap(int bufferCount);

	friend class RTex;
	friend class RenderTargetManager;
};

class RTex
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//ビューポート
	CD3DX12_VIEWPORT	viewport;
	//シザー矩形（レンダリング結果を取得する範囲）
	CD3DX12_RECT		rect;
	//クリアカラー
	std::array<float, 4> clearColors;

	//コンストラクタ
	RTex();
	//デストラクタ
	~RTex();

	/// <summary>
	/// レンダーテクスチャ生成
	/// </summary>
	/// <param name="texture_width">テクスチャ幅</param>
	/// <param name="texture_height">テクスチャ高さ</param>
	/// <param name="clearColor">クリアカラー</param>
	/// <param name="bufferCount">テクスチャ生成数</param>
	/// <param name="depthCount">深度テクスチャ生成数</param>
	/// <param name="option">レンダーテクスチャ1枚あたりの設定</param>
	void CreateRTex(int texture_width, int texture_height, float* clearColor, int bufferCount, RenderTextureOption* option = nullptr);

	//レンダーテクスチャのデータ取得
	const RenderTextureData* GetRTData() { return rtdata.get(); }

	ID3D12Resource *GetTextureBuffer(int arrayNum = 0) { return rtdata->rtexBuff[arrayNum].Get(); }

	ID3D12Resource* GetDepthBuffer() { return rtdata->depthBuff.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapRTV() { return rtdata->rtvHeap.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapDSV() { return rtdata->dsvHeap.Get(); }

	ID3D12DescriptorHeap* GetDescriptorHeapSRV() { return rtdata->srvHeap.Get(); }

	void ClearRenderTarget();

	friend class RenderTargetManager;

private:
	//レンダーテクスチャデータポインタ
	std::shared_ptr<RenderTextureData> rtdata;

	//画像サイズ（取得するにはmetadetaを参照しないといけないので保存しておく）
	std::pair<int, int> graph_size;

	//ビューポートとシザー矩形初期化
	void InitViewAndRect(int width, int height);

};

