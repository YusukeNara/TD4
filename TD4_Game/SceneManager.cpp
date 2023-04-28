#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"

//Raki_DX12B         *SceneManager::dx12b  = nullptr;
//NY_Object3DManager *SceneManager::objmgr = nullptr;
//SpriteManager      *SceneManager::spmgr  = nullptr;

SceneManager::SceneManager() :mNextScene(eScene_None) {

    //各シーンのインスタンス生成
    nowScene = (BaseScene *) new TitleScene(this);
    Initialize();
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
    if (mNextScene != eScene_None) {    //次のシーンがセットされていたら
        delete nowScene;
        nowScene = nullptr;
        
        switch (mNextScene) {       //シーンによって処理を分岐
        case eScene_Title:        //次の画面がメニューなら
            nowScene = (BaseScene*) new TitleScene(this);
            break;//以下略
        case eScene_Game:
            nowScene = (BaseScene*) new GameScene(this);
            break;
        }
        mNextScene = eScene_None;    //次のシーン情報をクリア
    }

    nowScene->Update(); //シーンの更新
}

void SceneManager::Draw()
{
    //現在シーンの描画処理を実行
    nowScene->Draw();
}

void SceneManager::Draw2D()
{
    nowScene->Draw2D();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //次のシーン番号をセット
    mNextScene = NextScene;
}
