#pragma once

#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>

#include "Head.h"
#include "ProtoPlayer.h"
#include"HeadManager.h"

//ゲームフロー管理クラス
class GameManager
{
public:

	GameManager();
	~GameManager();

	void Init();

	void Update();

	void Draw();

	void DrawParticle();

	void UIDraw();

	void Finalize();

	//スポーン管理関数

	//初回スポーン
	//void FirstSpawn();
	//ゲーム中スポーン管理
	void SpawnManagement();




private:
	//これらはアルファでクラス分けする

	/// スポーン管理

	//Head* HeadSpawn();
	
	////頭コンテナ
	//std::vector<std::shared_ptr<Head>> heads;

	////表示最大数
	//const int HEAD_DISPLAY_MAX = 5;
	////イージング用座標
	//std::array<RVector3, 5> easepos;

	std::unique_ptr<HeadManager> headMan;

	std::unique_ptr<ProtoPlayer> player;

	/// スコア




	/// タイム




	/// ゲームシーン




	//その他
	std::shared_ptr<Object3d> groundObject;

	UINT groundTex;

};

