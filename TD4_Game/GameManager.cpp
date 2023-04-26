#include "GameManager.h"

#include "AfroHead.h"

#include <NY_Object3DMgr.h>

GameManager::GameManager()
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));

}

GameManager::~GameManager()
{
}

void GameManager::Init()
{

	player.Init();

	RVector3 easeOffset(0, 0, 50.f);
	int i = 0;
	for (auto& ep : easepos) {
		ep = easeOffset * float(i);
		i++;
	}

	FirstSpawn();

	//時間
	time = 0;

	//スコア
	score = 0;





}

void GameManager::Update()
{
	//各クラス更新
	for (auto& h : heads) {
		h->Update();
	}

	//ノード先端の頭に対してアクションを実行する
	player.Update(heads[0]);

	//頭の整頓
	SpawnManagement();

	//頭の移動
	if (headEaseFrame < 30) {
		headEaseFrame++;
		float rate = float(headEaseFrame / HEAD_EASE_FRAME);
	}

}

void GameManager::Draw()
{
	NY_Object3DManager::Get()->SetRestartObject3D();

	//頭
	for (const auto& h : heads) { h->Draw(); }

	//床
	groundObject->DrawObject();

	NY_Object3DManager::Get()->CloseDrawObject3D();
}

void GameManager::UIDraw()
{
}

void GameManager::Finalize()
{
}

void GameManager::FirstSpawn()
{
	//5回スポーン、位置設定
	for (int i = 0; i < 5; i++) {

		Head* ptr = HeadSpawn();
		
		heads.push_back(std::make_shared<Head>());
		heads[i].reset(ptr);
		heads[i]->Init();
		heads[i]->SetPlayer(&player);
		heads[i]->pos = easepos[i];
	}
}

void GameManager::SpawnManagement()
{
	//頭の削除 => 新しい頭の生成 => 頭の移動 => 先頭の有効化

	bool isErasedHead = false;

	//左側境界線を超えたら、頭を削除する
	if (heads.begin()->get()->pos.x < -100.f) {
		heads.erase(heads.begin());
		isErasedHead = true;
	}

	//頭が削除されていたら
	if (isErasedHead) {
		//新しい頭の生成
		Head* ptr = HeadSpawn();
		heads.push_back(std::make_shared<Head>());
		heads.back()->Init();
		heads.back()->pos = easepos[4];

		//頭を移動させるためのイージングフレームを初期化
		headEaseFrame = 0;
	}

	//頭の移動が完了したら
	if (headEaseFrame > HEAD_EASE_FRAME) {
		//先頭の頭を有効化
		heads[0]->Activate();

	}



}

Head* GameManager::HeadSpawn()
{
	Head* head;

	//ランダムで頭を生成
	head = new AfroHead();

	return head;
}
