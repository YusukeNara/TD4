#include<iostream>

#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"
#include <RenderTargetManager.h>
#include <DifferrdRenderingMgr.h>
#include "SceneManager.h"

#include "NY_Object3DMgr.h"
#include "SpriteManager.h"
#include "Raki_imguiMgr.h"
#include <NY_Camera.h>

#include "GameManager.h"
#include "GameSoundMgr.h"
#include "FieldDrawer.h"

using namespace DirectX;
using namespace Microsoft::WRL;

//-----------RakiEngine_Alpha.ver-----------//


#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{

#pragma region GameValue

	Raki_WinAPI* rakiWinApp;
	rakiWinApp = new Raki_WinAPI;
	rakiWinApp->CreateGameWindow();

	//エンジン側のエラー、警告を無視しない設定にするときは、この関数の第二引数にtrueを渡すと良い
	Raki_DX12B::Get()->Initialize(rakiWinApp, true);

	myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

	
	NY_Object3DManager::Get()->CreateObject3DManager();
	SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), Raki_DX12B::Get()->GetGCommandList(),
		rakiWinApp->window_width, rakiWinApp->window_height);
	TexManager::InitTexManager();

	
	Audio::Init();

	DiferredRenderingMgr diffMgr;
	diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);


	RVector3 eye(0.f, 100.f, -100.f);
	RVector3 target(0.f, 0.f, 0.f);
	RVector3 up(0.f, 1.f, 0.f);
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

	FieldDrawer::get()->Init();

	std::unique_ptr<SceneManager> sceneMgr = std::make_unique<SceneManager>();


	//GameSoundMgr::get()->Init();


#pragma endregion GameValue

	FPS::Get()->Start();

	while (true)
	{
		if (rakiWinApp->ProcessMessage()) { break; }


		Input::StartGetInputState();

        sceneMgr->Update();


		RenderTargetManager::GetInstance()->CrearAndStartDraw();

		NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

		sceneMgr->Draw();

		FieldDrawer::get()->Draw();

		NY_Object3DManager::Get()->CloseDrawObject3D();

		diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer, &NY_Object3DManager::Get()->m_shadomMap);

		SpriteManager::Get()->SetCommonBeginDraw();

		sceneMgr->Draw2D();


		sceneMgr->DrawImgui();

		//描画ここまで

		RenderTargetManager::GetInstance()->SwapChainBufferFlip();

		FPS::Get()->run();
	}



	
	myImgui::FinalizeImGui();

	
	rakiWinApp->DeleteGameWindow();

	delete rakiWinApp;
	rakiWinApp = nullptr;

	return 0;
}