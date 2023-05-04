#include "GameScene.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {

	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));

	testsp.Create(testTex);
}

GameScene::~GameScene()
{
	if (testobject) {
		std::cout << "Object deleted" << endl;
	}
}

//‰Šú‰»
void GameScene::Initialize() {

}

void GameScene::Finalize()
{
}

//XV
void GameScene::Update() {
	if (Input::Get()->isKeyTrigger(DIK_1)) {
		mSceneChanger->ChangeScene(eScene_Title);
	}
}

//•`‰æ
void GameScene::Draw() {
	testobject->DrawObject();
}

void GameScene::Draw2D()
{
	testsp.DrawSprite(640, 360);
	testsp.Draw();
}
