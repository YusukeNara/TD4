#include "TitleScene.h"
#include <iostream>

#include "FieldDrawer.h"

using namespace Rv3Ease;

TitleScene::TitleScene(ISceneChanger* changer, SceneChangeDirection* scd) : BaseScene(changer, scd) {
	UINT testTex = TexManager::LoadTexture("Resources/titlemojitunage.png");
	UINT startTex = TexManager::LoadTexture("Resources/Start.png");
	UINT tutorialTex = TexManager::LoadTexture("Resources/TutorialStart.png");
	UINT buttonATex = TexManager::LoadTexture("Resources/buttonA.png");
	UINT buttonBTex = TexManager::LoadTexture("Resources/buttonB.png");

	titleHage = std::make_shared<Object3d>();
	titleHage.reset(LoadModel_FBXFile("hage_1"));
	titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(-30, 0, 0));
	titleHage->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 8);
	titlePlayer = std::make_shared<Object3d>();
	titlePlayer.reset(LoadModel_FBXFile("hage_1"));
	titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(30, 0, 0));
	titlePlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 2);

	titleAfro = std::make_shared<Object3d>();
	titleAfro.reset(LoadModel_FBXFile("kamihusahusa"));
	titleAfro->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(30, 5, 0));

	titleIppon = std::make_shared<Object3d>();
	titleIppon.reset(LoadModel_FBXFile("ippon"));
	titleIppon->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(330, 32, 0));


	titleEase.Init(RV3_EASE_TYPE::EASE_CUBIC_INOUT, RVector3(0, -200, 0),
		RVector3(0, 200, 0), 100);

	titleObjPos = { 0,0,0 };
	titlePlayerPos = { 50,0,0 };
	titleHagePos = { -50,0,0 };
	titleSpritePos = { -220,-400 };

	testsp.Create(testTex);
	startsp.Create(startTex);
	tutorialsp.Create(tutorialTex);
	buttonAsp.Create(buttonATex);
	buttonBsp.Create(buttonBTex);

	FieldDrawer::get()->SetTitleMode();

	isScroll = false;
	isTitleScroll = false;
}

TitleScene::~TitleScene()
{
	if (titleHage) {
		std::cout << "Hage deleted" << endl;
	}
	if (titlePlayer) {
		std::cout << "player deleted" << endl;
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
	if (isTitleScroll == true)
	{
		if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B)
			|| Input::Get()->isKeyTrigger(DIK_2))
		{
			isScroll = true;
		}
		if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_A)
			|| Input::Get()->isKeyTrigger(DIK_3)) {
			mSceneChanger->ChangeScene(eScene_Tutorial);
		}
	}

	if (isScroll)
	{
		SceneScroll();
	}
	else
	{
		Animation();
	}
}

//•`‰æ
void TitleScene::Draw() {
	//testobject->DrawObject();
	titleHage->DrawObject();
	titlePlayer->DrawObject();
	if ((animationCount != animationCountMax) && isTitleScroll == false)
	{
		titleAfro->DrawObject();
		titleIppon->DrawObject();
	}
}

void TitleScene::Draw2D()
{
	testsp.DrawSprite(titleSpritePos.x, titleSpritePos.y);
	testsp.Draw();
	if (titleSpritePos.y == titleLastPos)
	{
		startsp.DrawSprite(titleSpritePos.x + 800, titleSpritePos.y + 500.0f);
		buttonBsp.DrawSprite(titleSpritePos.x + 700, titleSpritePos.y + 500.0f);
		tutorialsp.DrawSprite(titleSpritePos.x + 500, titleSpritePos.y + 580.0f);
		buttonAsp.DrawSprite(titleSpritePos.x + 400, titleSpritePos.y + 580.0f);
		startsp.Draw();
		tutorialsp.Draw();
		buttonAsp.Draw();
		buttonBsp.Draw();
	}
}

void TitleScene::DrawImgui()
{
}

void TitleScene::Animation()
{
	if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B)
		|| Input::Get()->isKeyTrigger(DIK_2))
	{
		titleSpritePos.y = titleLastPos;
		titlePlayerPos = { 50,0,0 };
		titleHagePos = { -50,0,0 };
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
		animationCount = animationCountMax;
	}
	if (animationCount < animationCountMax)
	{
		animationCount++;
	}
	else
	{
		isTitleScroll = true;
		animationCount = 0;
	}
	if (animationCount < animationCountMax / 4)
	{
		titlePlayerPos.x += 2.0f;
		titleHagePos.x += 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
		if (isTitleScroll == false)
		{
			titleAfro->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(titlePlayerPos.x + 20, 5, 0));
		}
	}
	else if (animationCount < (animationCountMax / 4 * 3))
	{
		titleHage->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 2);
		titlePlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 8);
		titlePlayerPos.x -= 2.0f;
		titleHagePos.x -= 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titleHagePos);
		if (isTitleScroll == false)
		{
			titleIppon->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(titleHagePos.x - 18, 32, 0));
		}
	}
	else if (animationCount < animationCountMax)
	{
		titleHage->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 8);
		titlePlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 2);
		titlePlayerPos.x += 2.0f;
		titleHagePos.x += 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
	}

	if (isTitleScroll == true)
	{
		if (titleSpritePos.y < titleLastPos)
		{
			titleSpritePos.y += 2;
		}
	}
}

void TitleScene::SceneScroll()
{
	if (scrollCount < ScrollCountMax)
	{
		scrollCount++;
		titleSpritePos.y -= 10;
	}
	else
	{
		mSceneChanger->ChangeScene(eScene_Game);
	}
}