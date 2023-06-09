#include "GameScene.h"

#include "FieldDrawer.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {

	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	/*testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 0, 0), RVector3(0, 0, 0));*/

	testsp.Create(testTex);

	FieldDrawer::get()->SetGameMode();
	GameFrame = 0;
}

GameScene::~GameScene()
{
	/*if (testobject) {
		std::cout << "Object deleted" << endl;
	}*/
}

//‰Šú‰»
void GameScene::Initialize() {
	gmgr.Init();
}

void GameScene::Finalize()
{
}

//XV
void GameScene::Update() {
	GameFrame++;

	if (Input::Get()->isKeyTrigger(DIK_1)) {
		mSceneChanger->ChangeScene(eScene_Title);
	}

	if (60 - (GameFrame / 60) < 0) {
		mSceneChanger->ChangeScene(eScene_Result);
	}

	gmgr.Update();
}

//•`‰æ
void GameScene::Draw() {
	//testobject->DrawObject();

	gmgr.Draw();
}

void GameScene::Draw2D()
{
	testsp.DrawSprite(640, 360);
	//testsp.Draw();

	gmgr.UIDraw();
}

void GameScene::DrawImgui()
{
	ImguiMgr::Get()->StartDrawImgui("time", 100, 100);

	ImGui::Text("time : %d", 60 - (GameFrame / 60));

	ImguiMgr::Get()->EndDrawImgui();



}
