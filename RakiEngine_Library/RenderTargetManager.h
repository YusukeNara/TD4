#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <array>
#include <vector>
#include <iostream>

#include "RTex.h"
#include "Raki_imguiMgr.h"

/// <summary>
/// <para>レンダーターゲットを管理するクラス</para>
/// <para>バックバッファの管理も行うが、一番の目的はテクスチャ利用のためのレンダーターゲット作成</para>
/// <para>RenderTargetクラスの動向を管理することで、マルチパスレンダリングや</para>
/// <para>いずれ行うマルチレンダーターゲットの実装をよりわかりやすく、使いやすくするために存在</para>
/// <para>エンジンのフレームワーク化を控えてるため、シングルトンは使わない</para>
/// </summary>
class RenderTargetManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//デバイスを参照
	ID3D12Device* dev;
	//グラフィックスコマンドリストを参照
	ID3D12GraphicsCommandList* cmdlist;

public:
	//コンストラクタ
	RenderTargetManager();
	//デストラクタ
	~RenderTargetManager();

	//テスト用
	static RenderTargetManager* GetInstance()
	{
		static RenderTargetManager ins;
		return &ins;
	}

	//初期化
	void InitRenderTargetManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmd);

	//終了
	void FinalizeRenderTargetManager();

	//チェック用関数群

	//レンダーテクスチャハンドルが空かどうか？
	bool isNullHandle(int handle) { return renderTextures[handle] == nullptr; }

	//レンダーテクスチャハンドルが範囲外か？
	bool isHandleOutOfRange(int handle) { return handle < 0 || handle > renderTextures.size(); }

	//描画の開始（バックバッファをクリアして、描画コマンド実行開始）
	void CrearAndStartDraw();


	/// <summary>
	/// レンダーテクスチャの生成
	/// </summary>
	/// <param name="width">テクスチャ横幅</param>
	/// <param name="height">テクスチャ縦幅</param>
	/// <returns>生成したレンダーテクスチャのハンドル</returns>
	int CreateRenderTexture(int width, int height);


	/// <summary>
	/// レンダーテクスチャをレンダーターゲットに設定
	/// </summary>
	/// <param name="handle">レンダーテクスチャのハンドル</param>
	void SetRenderTarget(int handle);

	void SetRenderTarget(RTex* rt, int handle, bool clearFlag = false);

	/// <summary>
	/// レンダーターゲット複数設定（現状ディファードレンダリング用）
	/// </summary>
	/// <param name="renderTargets">レンダーテクスチャの配列</param>
	void SetMultiRenderTargets(const RTex* renderTargets,int size, bool clearFlag = false);

	void SetRenderTargetForRTexSpecifiedIndex(const RTex* renderTargets, int targetIndex, bool clearFlag = false);

	

	/// <summary>
	/// ディファードレンダリングで設定したレンダーターゲットをクローズ
	/// </summary>
	/// <param name="renderTargets">レンダーターゲット配列</param>
	/// <param name="size">レンダーターゲットの枚数</param>
	void CloseMultiRenderTargets(const RTex *renderTargets, int size, bool isChangeBB = false);

	void CloseMultiRenderTargetsForRTexSpecifiedIndex(const RTex* renderTargets, int targetIndex, bool isChangeBB = false);


	/// <summary>
	/// レンダーテクスチャのシザー矩形を設定する
	/// <para>ここで指定したレンダーテクスチャは、ここで指定したスクリーン座標範囲外を描画しなくなる</para>
	/// <para>ただし右下座標は、予め設定した画像サイズより大きくなるような値にはならない</para>
	/// </summary>
	/// <param name="handle">設定するレンダーテクスチャハンドル</param>
	/// <param name="x1">シザー矩形左</param>
	/// <param name="y1">シザー矩形下</param>
	/// <param name="x2">シザー矩形右（画像サイズより大きい場合は自動で修正される）</param>
	/// <param name="y2">シザー矩形下（画像サイズより大きい場合は自動で修正される）</param>
	void SetRenderTargetDrawArea(int handle, int x1, int y1, int x2, int y2);


	/// <summary>
	/// レンダーテクスチャのビューポートを設定する
	/// <para>ここで指定したレンダーテクスチャは、ここで設定したビューポートの範囲内に描画される</para>
	/// <para>ビューポートについては有効活用できる場面が限られているので、そこまで気にする必要はない。</para>
	/// </summary>
	/// <param name="handle">設定するレンダーテクスチャハンドル</param>
	/// <param name="x1">ビューポート左</param>
	/// <param name="y1">ビューポート上</param>
	/// <param name="x2">ビューポート右</param>
	/// <param name="y2">ビューポート下</param>
	void SetRenderTargetClipingArea(int handle, int x1, int y1, int x2, int y2);


	/// <summary>
	/// レンダーテクスチャの共通クリアカラーを設定
	/// </summary>
	/// <param name="red"></param>
	/// <param name="green"></param>
	/// <param name="blue"></param>
	void SetClearColor(float red, float green, float blue);

	void ClearRenderTexture(int handle);

	

	//バックバッファへの描画を開始（レンダーテクスチャへ描画している時）
	void SetDrawBackBuffer();

	//デプスのみを設定
	void SetDSV(RTex* gBuffer);

	//スワップチェーン用バッファをフリップし、現在フレームでの描画終了コマンド実行
	void SwapChainBufferFlip();

private:
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapchain;
	//取得
	IDXGISwapChain4* scptr;
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//バックバッファ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//バックバッファ用デプス用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//ビューポートとシザー矩形は、レンダーテクスチャを使用しない場合はデフォルトのものを使う
	CD3DX12_VIEWPORT	default_viewport;
	CD3DX12_RECT		default_rect;
	//レンダーテクスチャコンテナ
	std::vector<std::shared_ptr<RTex>> renderTextures;

	//現在の描画対象は？
	enum DRAW_TARGET
	{
		USING_BACKBUFFER,
		USING_RENDERTEXTURE,
	};
	DRAW_TARGET isDrawing;

	//背景クリアカラー
	float clearcolor[4] = { 0.1f,0.25f,0.5f,0.0f };

	//スワップチェーン生成関数
	void CreateSwapChain();

	//バックバッファ生成関数	
	void CreateBackBuffers();

	//深度バッファ生成
	void CreateDepthBuffer();

	//深度バッファクリア
	void ClearDepthBuffer(ID3D12DescriptorHeap *dsv);

	//レンダーターゲットクリア
	void ClearRenderTarget(ID3D12DescriptorHeap *rtv);

	void ClearBackBuffer();

	//現在レンダーターゲットにしているやつ
	int nowRenderTargetHandle = 0;

	//バックバッファをクローズ（描画終了ではない）
	void CloseDrawBackBuffer();

	//レンダーターゲットのリソースバリアを表示状態に
	void CloseDrawRenderTexture();

	//バッファフリップ
	void BufferFlip();

	friend class Sprite;
	friend class SpriteManager;
	friend class Object3d;
	friend class NY_Object3DManager;
};