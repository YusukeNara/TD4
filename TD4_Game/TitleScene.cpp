#include "TitleScene.h"
#include <iostream>

using namespace Rv3Ease;

TitleScene::TitleScene(ISceneChanger *changer) : BaseScene(changer) {
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");

	titleHage = std::make_shared<Object3d>();
	titleHage.reset(LoadModel_FBXFile("hage_1"));
	titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(-30, 0, 0));
	titleHage->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);
	titlePlayer = std::make_shared<Object3d>();
	titlePlayer.reset(LoadModel_FBXFile("hage_1"));
	titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(30, 0, 0));
	titlePlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);

	titleEase.Init(RV3_EASE_TYPE::EASE_CUBIC_INOUT, RVector3(400, -200, 0),
		RVector3(400, 200, 0), 100);

	testobject = std::make_shared<Object3d>();
	testobject.reset(NY_Object3DManager::Get()->CreateModel_Box(50.f, 10.f, 10.f, testTex));

	titleObjPos = { 0,0,0 };
	titlePlayerPos = { 30,0,0 };
	titleHagePos = { -30,0,0 };
	titleSpritePos = { 400,-200 };

	testobject->SetAffineParam(RVector3(1, 1, 1), RVector3(0, 45, 0), titleObjPos);

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
	//testobject->DrawObject();
	titleHage->DrawObject();
	titlePlayer->DrawObject();
}

void TitleScene::Draw2D()
{
	testsp.DrawSprite(titleSpritePos.x, titleSpritePos.y);
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
		if (titleSpritePos.y < titleLastPos)
		{
			titleSpritePos.y++;
		}
	}
	if (animationCount < animationCountMax / 4)
	{
		titlePlayerPos.x += 1.0f;
		titleHagePos.x += 1.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
	}
	else if (animationCount < (animationCountMax / 4 * 3))
	{
		titlePlayerPos.x -= 1.0f;
		titleHagePos.x -= 1.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titleHagePos);
	}
	else if (animationCount < animationCountMax)
	{
		titlePlayerPos.x += 1.0f;
		titleHagePos.x += 1.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
	}
}
