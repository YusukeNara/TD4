#pragma once
#include "SceneBase.h"
#include "SceneChanger.h"
#include "Task.h"

#include <vector>

class ISceneManager : public Task
{
private:
	//SceneBaseのポインタ型Listコンテナ
	static std::vector<SceneBase*> scenes;
	//現在のシーン番号
	static int sceneNum;
	//変更先シーン番号
	static int changeNum;

public:
	//コンストラクタ、デストラクタ
	ISceneManager();
	~ISceneManager();

	/// <summary>
	/// ユーザーが作成したシーンをシーンコンテナに追加
	/// </summary>
	/// <param name="scene">SceneBaseを継承して作ったシーンクラスのポインタ変数</param>
	/// <returns>格納したシーンの場所を示す番号</returns>
	int AddScene(SceneBase *scene);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	/// <summary>
	/// 実行シーンを変更
	/// </summary>
	/// <param name="nextSceneNum"></param>
	void ChangeScene(int nextSceneNum);

};

