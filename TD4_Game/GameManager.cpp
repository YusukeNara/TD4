#include "GameManager.h"

#include "AfroHead.h"

#include <NY_Object3DMgr.h>

GameManager::GameManager()
{
	groundTex = TexManager::LoadTexture("Resources/asp3.png");

	groundObject = std::make_shared<Object3d>();
	groundObject.reset(NY_Object3DManager::Get()->CreateModel_Tile(500.f, 500.f, 10.f, 10.f, groundTex));

	groundObject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));


	player = std::make_unique<ProtoPlayer>();
	headMan = std::make_unique<HeadManager>(player.get());
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	player->Init();

	RVector3 easeOffset(0, 0, 100.f);
	headMan->Initialize();
}

void GameManager::Update()
{
	player->Update();

	headMan->Update();
}

void GameManager::Draw()
{
	//NY_Object3DManager::Get()->SetRestartObject3D();

	headMan->Draw();

	//床
	//groundObject->DrawObject();

	//NY_Object3DManager::Get()->CloseDrawObject3D();
}

void GameManager::UIDraw()
{
	player->DrawUI();
}

void GameManager::Finalize()
{
}