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

using namespace DirectX;
using namespace Microsoft::WRL;

//-----------RakiEngine_Alpha.ver-----------//


//コンソール表示用エントリーポイント切り替え
//Releaseでもコンソールしたい場合、プロパティ->リンカー->サブシステムをコンソールに切り替えてから、WinMainをmainにする
#ifdef _DEBUG
int main()
#else
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{

#pragma region GameValue

    Raki_WinAPI* rakiWinApp;
    rakiWinApp = new Raki_WinAPI;
    rakiWinApp->CreateGameWindow();

    //エンジン側のエラー、警告を無視しない設定にするときは、この関数の第二引数にtrueを渡すと良い
    Raki_DX12B::Get()->Initialize(rakiWinApp, false);

    myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

    //オブジェクト管理
    NY_Object3DManager::Get()->CreateObject3DManager();
    SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), Raki_DX12B::Get()->GetGCommandList(),
        rakiWinApp->window_width, rakiWinApp->window_height);
    TexManager::InitTexManager();

    //音
    Audio::Init();

    DiferredRenderingMgr diffMgr;
    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

    //シーン管理
    RVector3 eye(0.f, 100.f, -100.f);
    RVector3 target(0.f, 0.f, 0.f);
    RVector3 up(0.f, 1.f, 0.f);
    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);



    std::unique_ptr<SceneManager> sceneMgr = std::make_unique<SceneManager>();




#pragma endregion GameValue

    FPS::Get()->Start();

    while (true)  // ゲームループ
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //更新
        Input::StartGetInputState();


        //更新ここまで

        sceneMgr->Update();

        //描画ここから
        RenderTargetManager::GetInstance()->CrearAndStartDraw();

        NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

        sceneMgr->Draw();

        NY_Object3DManager::Get()->CloseDrawObject3D();

        diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer, &NY_Object3DManager::Get()->m_shadomMap);

        SpriteManager::Get()->SetCommonBeginDraw();

        sceneMgr->Draw2D();

        sceneMgr->DrawImgui();

        //描画ここまで
        RenderTargetManager::GetInstance()->SwapChainBufferFlip();

        FPS::Get()->run();
    }



    //imgui終了
    myImgui::FinalizeImGui();

    // ウィンドウクラスを登録解除
    rakiWinApp->DeleteGameWindow();

    delete rakiWinApp;
    rakiWinApp = nullptr;

    return 0;
}