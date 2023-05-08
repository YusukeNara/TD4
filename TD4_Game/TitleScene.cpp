#include "TitleScene.h"
#include <iostream>

TitleScene::TitleScene(ISceneChanger *changer) : BaseScene(changer) {
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 45, 0), RVector3(0, 0, 0));

	testsp.Create(testTex);
}

TitleScene::~TitleScene()
{
	if (testobject) {
		std::cout << "Object deleted" << endl;
	}
}

//‰Šú‰»
void TitleScene::Initialize() {

}

void TitleScene::Finalize()
{

}

//XV
void TitleScene::Update() {

	if (Input::Get()->isKeyTrigger(DIK_2)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
}

//•`‰æ
void TitleScene::Draw() {
	testobject->DrawObject();
}

void TitleScene::Draw2D()
{
	testsp.DrawSprite(0, 0);
	testsp.Draw();
}
