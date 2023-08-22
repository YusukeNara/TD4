#include<iostream>

#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"
#include <RenderTargetManager.h>
#include <DifferrdRenderingMgr.h>
#include "SceneManager.h"
#include <GraphicManager.h>

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
	Raki_DX12B::Get()->Initialize(rakiWinApp, false);

	GraphicManager graphicmgr;
	graphicmgr.Init();
	
	Audio::Init();

	RVector3 eye(0.f, 100.f, -100.f);
	RVector3 target(0.f, 0.f, 0.f);
	RVector3 up(0.f, 1.f, 0.f);
	NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

	FieldDrawer::get()->Init();

	GameSoundMgr::get()->Init();

	std::unique_ptr<SceneManager> sceneMgr = std::make_unique<SceneManager>();


#pragma endregion GameValue

	FPS::Get()->Start();

	while (true)
	{
		if (rakiWinApp->ProcessMessage()) { break; }

		Input::StartGetInputState();

        sceneMgr->Update();

		graphicmgr.StartDraw();
		//3D通常描画ここから
		graphicmgr.StartDeferredDraw();

		FieldDrawer::get()->Draw();

		sceneMgr->Draw();

		//3D通常描画ここまで
		graphicmgr.EndDefferdDraw();

		//パーティクル3Dここから
		graphicmgr.StartParticleDraw3D();

		sceneMgr->DrawParticle();

		//スプライトここから
		graphicmgr.StartSpriteDraw();

		sceneMgr->Draw2D();

		sceneMgr->DrawImgui();

		graphicmgr.GraphicDebugDraw();

		//描画ここまで

		graphicmgr.FinishDraw();

		FPS::Get()->run();
	}
	
	myImgui::FinalizeImGui();

	Raki_DX12B::Get()->Finalize();
	
	rakiWinApp->DeleteGameWindow();

	delete rakiWinApp;
	rakiWinApp = nullptr;

	return 0;
}