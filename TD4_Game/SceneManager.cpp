#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "EngineDebugScene.h"
#include "ResultScene.h"
#include"TutorialScene.h"

//Raki_DX12B         *SceneManager::dx12b  = nullptr;
//NY_Object3DManager *SceneManager::objmgr = nullptr;
//SpriteManager      *SceneManager::spmgr  = nullptr;

SceneManager::SceneManager() :mNextScene(eScene_None) {
    if (!mSceneChangeDirection) { 
        mSceneChangeDirection.reset(new SceneChangeDirection());
        mSceneChangeDirection->Init();
    }

    //各シーンのインスタンス生成
    nowScene = (BaseScene *) new EngineDebugScene(this,mSceneChangeDirection.get());
    Initialize();
}

SceneManager::~SceneManager()
{
    delete nowScene;
    nowScene = nullptr;
}

void SceneManager::Initialize()
{
    //シーンの初期化
	nowScene->Initialize();
}

void SceneManager::Finalize()
{
    nowScene->Finalize();
}

void SceneManager::Update()
{
    //デバッグモード
    if (Input::Get()->isKeyTrigger(DIK_F12)) { mNextScene = eScene_Debug; }
    if(Input::Get()->isKeyTrigger(DIK_F1)) { mNextScene = eScene_Title; }

    if (mNextScene != eScene_None) {    //次のシーンがセットされていたら

        if (!isChangeDirectioning) {
            //シーン終了演出を再生
            mSceneChangeDirection->PlayOutDirection();
            isChangeDirectioning = true;
        }
        //シーン演出終了時
        if (mSceneChangeDirection->GetDirectionStatus() == DIRECTION_ENDED && isChangeDirectioning) {
            delete nowScene;
            nowScene = nullptr;

            switch (mNextScene) {       //シーンによって処理を分岐
            case eScene_Title:        //次の画面がメニューなら
                nowScene = (BaseScene*) new TitleScene(this, mSceneChangeDirection.get());
                break;//以下略
            case eScene_Tutorial:
                nowScene = (BaseScene*) new TutorialScene(this, mSceneChangeDirection.get());
                break;
            case eScene_Game:
                nowScene = (BaseScene*) new GameScene(this, mSceneChangeDirection.get());
                break;
            case eScene_Result:
                nowScene = (BaseScene*) new ResultScene(this, mSceneChangeDirection.get());
                break;
            case eScene_Debug:
                nowScene = (BaseScene*) new EngineDebugScene(this, mSceneChangeDirection.get());
                break;
            }
            if (mNextScene != eScene_Debug) {
                mSceneChangeDirection->PlayInDirection();
            }

            mNextScene = eScene_None;    //次のシーン情報をクリア
            isChangeDirectioning = false;
            nowScene->Initialize();
        }
    }

    nowScene->Update(); //シーンの更新

    mSceneChangeDirection->Update();
}

void SceneManager::Draw()
{
    //現在シーンの描画処理を実行
    nowScene->Draw();
}

void SceneManager::Draw2D()
{
    nowScene->Draw2D();

    mSceneChangeDirection->Draw();
}

void SceneManager::DrawImgui()
{
    nowScene->DrawImgui();
}

void SceneManager::DrawParticle()
{
    nowScene->DrawParticle();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //次のシーン番号をセット
    mNextScene = NextScene;
}
