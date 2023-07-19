#include "SceneChangeDirection.h"


SceneChangeDirection::SceneChangeDirection()
{
	spriteBlack.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/blackParticleTex.png"));
	spritePos = { 0,0 };
	spriteSize = { 100,100 };
}

SceneChangeDirection::~SceneChangeDirection()
{
}

void SceneChangeDirection::Init()
{
}

void SceneChangeDirection::PlayInDirection()
{
	if (isAnimation == false)
	{
		animationCount = 0;
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
		{
			isAnimation = true;
		}
	}
}

void SceneChangeDirection::PlayOutDirection()
{
	if (isAnimation == true)
	{
		if (animationCount < animationCountMax)
		animationCount++;
		spriteSize.x += 2;
		spriteSize.y += 2;
	}
}

void SceneChangeDirection::Update()
{
	PlayInDirection();
	PlayOutDirection();
}

void SceneChangeDirection::Draw()
{
	spriteBlack.DrawExtendSprite(spritePos.x - spriteSize.x, spritePos.y - spriteSize.y, spritePos.x + spriteSize.x, spritePos.y + spriteSize.y);
	spriteBlack.Draw();
}
