#include "GraphicManager.h"
#include "Raki_WinAPI.h"

void GraphicManager::Init()
{
	//各フェーズの初期化
	NY_Object3DManager::Get()->CreateObject3DManager();
	SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(),
		Raki_DX12B::Get()->GetGCommandList(),
		Raki_WinAPI::window_width,
		Raki_WinAPI::window_height);

	TexManager::InitTexManager();

	myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

	//ディファードレンダリング初期化
	m_deferredRender.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

	//パーティクル描画初期化
	m_particleDrawMgr.Init();
}

void GraphicManager::StartDraw()
{
	//レンダーターゲットクリア、描画開始
	RenderTargetManager::GetInstance()->CrearAndStartDraw();
}

void GraphicManager::StartDeferredDraw()
{
	//GBuffer描画開始
	NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
}

void GraphicManager::EndDefferdDraw()
{
	//GBuffer描画終了
	NY_Object3DManager::Get()->CloseDrawObject3D();
	//ディファードレンダリング実行（シャドウマップ未使用）
	m_deferredRender.Rendering(&NY_Object3DManager::Get()->m_gBuffer, nullptr);
}

void GraphicManager::StartForwardDraw3D()
{
	//フォワードレンダリングのシェーダーを用意するまではこの関数は意味無し
}

void GraphicManager::StartParticleDraw3D()
{
	//
	m_particleDrawMgr.SetCommonBeginDrawParticle3D();
}

void GraphicManager::StartParticleDraw2D()
{
	//m_particleDrawMgr.SetCommonBeginDrawParticle2D();
}

void GraphicManager::StartSpriteDraw()
{
	SpriteManager::Get()->SetCommonBeginDraw();
}

void GraphicManager::FinishDraw()
{
	RenderTargetManager::GetInstance()->SwapChainBufferFlip();
}
