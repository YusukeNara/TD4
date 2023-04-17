#include "ISceneManager.h"

#include <assert.h>

std::vector<SceneBase *> ISceneManager::scenes;
int ISceneManager::sceneNum = 0;
int ISceneManager::changeNum = 0;

ISceneManager::ISceneManager()
{
	
}

ISceneManager::~ISceneManager()
{
	//全シーンの終了
	Finalize();
}

int ISceneManager::AddScene(SceneBase *scene)
{
	//引数のシーンをlistに格納
	scenes.push_back(scene);

	//基本末尾に追加するため、末尾の位置を取得して返却
	return scenes.size() - 1;
}

void ISceneManager::Initialize()
{
	//シーンが格納されていない場合はエラー
	assert(scenes.size() == 0);
	//初回実行するシーンは先頭のシーン
	changeNum = 0;
	sceneNum = 0;
	//シーン初期化
	scenes[sceneNum]->Initialize();
}

void ISceneManager::Update()
{
	//シーン変更を検知
	if (changeNum != sceneNum) {
		//現在シーンの終了処理を実行
		scenes[sceneNum]->Finalize();
		//次シーンの初期化処理を実行
		scenes[changeNum]->Initialize();
		//実行シーン番号を変更
		sceneNum = changeNum;
	}
	//シーンの更新処理を実行
	scenes[sceneNum]->Update();
}

void ISceneManager::Draw()
{
	//実行シーンの描画処理を実行
	scenes[sceneNum]->Draw();
}

void ISceneManager::Finalize()
{
	//全シーンの終了処理を実行
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i]->Finalize();
	}
}

void ISceneManager::ChangeScene(int nextSceneNum)
{
	//変更先シーン番号を設定
	changeNum = nextSceneNum;
}
