#include "TitleScene.h"
#include <iostream>

#include "FieldDrawer.h"

using namespace Rv3Ease;

TitleScene::TitleScene(ISceneChanger *changer, SceneChangeDirection* scd) : BaseScene(changer,scd) {
	UINT testTex = TexManager::LoadTexture("Resources/titlemojitunage.png");

	titleHage = std::make_shared<Object3d>();
	titleHage.reset(LoadModel_FBXFile("hage_1"));
	titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(-30, 0, 0));
	titleHage->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);
	titlePlayer = std::make_shared<Object3d>();
	titlePlayer.reset(LoadModel_FBXFile("hage_1"));
	titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(30, 0, 0));
	titlePlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);

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

	FieldDrawer::get()->SetTitleMode();
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
	if (animationCount == animationCountMax)
	{
		if (Input::Get()->isXpadButtonPushTrigger(XPAD_BUTTON_B)
			|| Input::Get()->isKeyTrigger(DIK_2))
		{
			isScroll = true;
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
	if (Input::Get()->isKeyTrigger(DIK_3)) {
		mSceneChangeDirection->PlayOutDirection();
	}
	if (mSceneChangeDirection->GetDirectionStatus() == DIRECTION_ENDED) {
		mSceneChanger->ChangeScene(eScene_Tutorial);
	}
}

//•`‰æ
void TitleScene::Draw() {
	//testobject->DrawObject();
	titleHage->DrawObject();
	titlePlayer->DrawObject();
	if ((animationCount != animationCountMax))
	{
		titleAfro->DrawObject();
		titleIppon->DrawObject();
	}
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
		if (titleSpritePos.y < titleLastPos)
		{
			titleSpritePos.y += 2;
		}
	}
	if (animationCount < animationCountMax / 4)
	{
		titlePlayerPos.x += 2.0f;
		titleHagePos.x += 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
		titleAfro->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(titlePlayerPos.x, 5, 0));
	}
	else if (animationCount < (animationCountMax / 4 * 3))
	{
		titlePlayerPos.x -= 2.0f;
		titleHagePos.x -= 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 180, 0), titleHagePos);
		titleIppon->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(titleHagePos.x, 32, 0));
	}
	else if (animationCount < animationCountMax)
	{
		titlePlayerPos.x += 2.0f;
		titleHagePos.x += 2.0f;
		titlePlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titlePlayerPos);
		titleHage->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), titleHagePos);
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
