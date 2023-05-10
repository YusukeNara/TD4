�ｿ#include<iostream>

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


//繧ｳ繝ｳ繧ｽ繝ｼ繝ｫ陦ｨ遉ｺ逕ｨ繧ｨ繝ｳ繝医Μ繝ｼ繝昴う繝ｳ繝亥�繧頑崛縺
//Release縺ｧ繧ゅさ繝ｳ繧ｽ繝ｼ繝ｫ縺励◆縺�ｴ蜷医√�繝ｭ繝代ユ繧｣->繝ｪ繝ｳ繧ｫ繝ｼ->繧ｵ繝悶す繧ｹ繝�Β繧偵さ繝ｳ繧ｽ繝ｼ繝ｫ縺ｫ蛻�ｊ譖ｿ縺医※縺九ｉ縲仝inMain繧知ain縺ｫ縺吶ｋ
#ifdef _DEBUG
int main()
#else
// Windows繧｢繝励Μ縺ｧ縺ｮ繧ｨ繝ｳ繝医Μ繝ｼ繝昴う繝ｳ繝(main髢｢謨ｰ)
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

    //繧ｪ繝悶ず繧ｧ繧ｯ繝育ｮ｡逅
    NY_Object3DManager::Get()->CreateObject3DManager();
    SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), Raki_DX12B::Get()->GetGCommandList(),
        rakiWinApp->window_width, rakiWinApp->window_height);
    TexManager::InitTexManager();

    //髻ｳ
    Audio::Init();

    DiferredRenderingMgr diffMgr;
    diffMgr.Init(RAKI_DX12B_DEV, RAKI_DX12B_CMD);

    //繧ｷ繝ｼ繝ｳ邂｡逅
    RVector3 eye(0.f, 100.f, -100.f);
    RVector3 target(0.f, 0.f, 0.f);
    RVector3 up(0.f, 1.f, 0.f);
    NY_Camera::Get()->SetViewStatusEyeTargetUp(eye, target, up);

    GameManager gmgr;

    std::unique_ptr<SceneManager> sceneMgr = std::make_unique<SceneManager>();


    gmgr.Init();

#pragma endregion GameValue

    FPS::Get()->Start();

    while (true)  // 繧ｲ繝ｼ繝繝ｫ繝ｼ繝
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //譖ｴ譁ｰ
        Input::StartGetInputState();


        //譖ｴ譁ｰ縺薙％縺ｾ縺ｧ

        gmgr.Update();

        sceneMgr->Update();

        //謠冗判縺薙％縺九ｉ
        RenderTargetManager::GetInstance()->CrearAndStartDraw();

        NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();

        sceneMgr->Draw();

        NY_Object3DManager::Get()->CloseDrawObject3D();

        diffMgr.Rendering(&NY_Object3DManager::Get()->m_gBuffer, &NY_Object3DManager::Get()->m_shadomMap);

        SpriteManager::Get()->SetCommonBeginDraw();

        sceneMgr->Draw2D();


        //謠冗判縺薙％縺ｾ縺ｧ

        sceneMgr->DrawImgui();

        //描画ここまで

        RenderTargetManager::GetInstance()->SwapChainBufferFlip();

        FPS::Get()->run();
    }



    //imgui邨ゆｺ
    myImgui::FinalizeImGui();

    // 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ繧堤匳骭ｲ隗｣髯､
    rakiWinApp->DeleteGameWindow();

    delete rakiWinApp;
    rakiWinApp = nullptr;

    return 0;
}