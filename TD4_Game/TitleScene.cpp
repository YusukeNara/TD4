#include "TitleScene.h"
#include <iostream>

TitleScene::TitleScene(ISceneChanger *changer) : BaseScene(changer) {
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");


	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	testObjPos = { 0,0,0 };

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 45, 0), testObjPos);

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
	Animation();
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

void TitleScene::DrawImgui()
{
}

void TitleScene::Animation()
{
	if (animationCount < animationCountMax)
	{
		animationCount++;
	}
	else
	{
	}
	if (animationCount < animationCountMax / 4)
	{
		testObjPos.x += 0.5f;
	}
	else if (animationCount < (animationCountMax / 4 * 3))
	{
		testObjPos.x -= 0.5f;
	}
	else if (animationCount < animationCountMax)
	{
		testObjPos.x += 0.5f;
	}
	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 45, 0), testObjPos);
}
