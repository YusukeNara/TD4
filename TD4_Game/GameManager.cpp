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

	RVector3 easeOffset(0, 0, 100.f);
	int i = 0;
	for (auto& ep : easepos) {
		ep = easeOffset * float(i);
		i++;
	}

	FirstSpawn();

}

void GameManager::Update()
{
	for (auto& h : heads) {
		h->Update();
	}


}

void GameManager::Draw()
{
	NY_Object3DManager::Get()->SetRestartObject3D();

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
		heads[i]->pos = easepos[i];

		
	}
}

void GameManager::SpawnManagement()
{
	//



}

Head* GameManager::HeadSpawn()
{
	Head* head;

	//ランダムで頭を生成
	head = new AfroHead();

	return head;
}
