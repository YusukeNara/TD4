#include "ResultScene.h"

ResultScene::ResultScene(ISceneChanger* changer) : BaseScene(changer)
{
	UINT testTex = TexManager::LoadTexture("Resources/asp3.png");
	resultPlayer = std::make_shared<Object3d>();
	resultPlayer.reset(LoadModel_FBXFile("hage_1"));
	resultPlayer->SetAffineParam(RVector3(0.2f, 0.2f, 0.2f), RVector3(0, 0, 0), RVector3(30, 0, 0));
	resultPlayer->PlayAnimation(ANIMATION_PLAYMODE::ANIM_MODE_ROOP, 0);
	starPos = { 200.0f,100.0f };
	for (int i = 0; i < 5; i++)
	{
		spriteStar[i].Create(testTex);
	}
	scoreTex.Create(testTex);
	starIsDraw = false;
	scoreTexIsDraw = false;
}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{

}

void ResultScene::Finalize()
{

}

void ResultScene::Update()
{
	Animation();
}

void ResultScene::Draw()
{
	resultPlayer->DrawObject();
}

void ResultScene::Draw2D()
{
	if (starIsDraw)
	{
		for (int i = 0; i < 5; i++)
		{
			spriteStar[i].DrawSprite(starPos.x, starPos.y);
			spriteStar[i].Draw();
		}
	}
}

void ResultScene::DrawImgui()
{

}

void ResultScene::Animation()
{
	if (animationCount < animationCountMax)
	{
		animationCount++;
	}
	if (animationCount < animationCount / 4)
	{
		starIsDraw = true;
	}
	else if (animationCount < animationCount / 2)
	{
		scoreTexIsDraw = true;
	}
}
