#pragma once

#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>

#include "Head.h"
#include "ProtoPlayer.h"

//ゲームフロー管理クラス
class GameManager
{
public:

	GameManager();
	~GameManager();

	void Init();

	void Update();

	void Draw();

	void UIDraw();

	void Finalize();

	//スポーン管理関数

	//初回スポーン
	void FirstSpawn();
	//ゲーム中スポーン管理
	void SpawnManagement();




private:
	//これらはアルファでクラス分けする

	/// スポーン管理

	Head* HeadSpawn();
	
	//頭コンテナ
	std::vector<std::shared_ptr<Head>> heads;

	//表示最大数
	const int HEAD_DISPLAY_MAX = 5;
	//イージング用座標
	std::array<RVector3, 5> easepos;

	//頭の移動フレーム
	const int HEAD_EASE_FRAME = 30;
	int headEaseFrame = 0;


	/// スコア
	int score = 0;



	/// タイム
	int time = 0;



	/// ゲームシーン
	ProtoPlayer player;



	//その他
	std::shared_ptr<Object3d> groundObject;

	UINT groundTex;

};

