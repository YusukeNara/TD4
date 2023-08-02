#include "SceneChangeDirection.h"

using namespace Rv3Ease;

SceneChangeDirection::SceneChangeDirection()
{
	spriteBlack.CreateAndSetDivisionUVOffsets(10, 10, 1, 64, 64, TexManager::LoadTexture("Resources/white1x1.png"));
	spritePos = { 0,0 };
	spriteSize = { 100,100 };
	status = DIRECTION_IN;

	inDirect.Init(RV3_EASE_TYPE::EASE_QUAD_IN,
		WINDOW_CENTER,
		RVector3(0, 0, 0),
		60);
	outDirect.Init(RV3_EASE_TYPE::EASE_QUAD_OUT,
		RVector3(0, 0, 0),
		WINDOW_CENTER,
		60);
}

SceneChangeDirection::~SceneChangeDirection()
{
}

void SceneChangeDirection::Init()
{
}

void SceneChangeDirection::PlayInDirection()
{
	drawingStatus = DIRECTION_IN;
	status = DIRECTION_IN;
	animationCount = 0;
	spritePos = { 0,0 };
	spriteSize = { 500,500 };
	drawingStatus = status;

	inDirect.Reset();
	inDirect.Play();
}

void SceneChangeDirection::PlayOutDirection()
{
	drawingStatus = DIRECTION_OUT;
	status = DIRECTION_OUT;
	animationCount = 0;
	spritePos = { 0,0 };
	spriteSize = { 100,100 };

	outDirect.Reset();
	outDirect.Play();
}

void SceneChangeDirection::Update()
{

	switch (status)
	{
	case DIRECTION_STANDBY:
		break;
	case DIRECTION_IN:
		//if (animationCount < animationCountMax) {
		//	animationCount++;
		//}
		//else if (animationCount >= animationCountMax) {
		//	status = DIRECTION_STANDBY;
		//}

		inDirect.Update();

		if (inDirect.IsEnd()) {
			status = DIRECTION_ENDED;
			outDirect.Reset();
		}

		break;
	case DIRECTION_OUT:
		//if (animationCount < animationCountMax) {
		//	animationCount++;
		//	spriteSize.x += 2;
		//	spriteSize.y += 2;
		//}
		//else if (animationCount >= animationCountMax) {
		//	status = DIRECTION_ENDED;
		//}

		outDirect.Update();

		if (outDirect.IsEnd()) {
			status = DIRECTION_ENDED;
			inDirect.Reset();
		}

		break;
	case DIRECTION_ENDED:
		break;
	default:
		break;
	}

	//PlayInDirection();
	//PlayOutDirection();
}

void SceneChangeDirection::Draw()
{

	if (drawingStatus == DIRECTION_IN) {
		spriteBlack.DrawExtendSprite(WINDOW_CENTER.x - inDirect.GetNowpos().x,
			WINDOW_CENTER.y - inDirect.GetNowpos().y,
			WINDOW_CENTER.x + inDirect.GetNowpos().x,
			WINDOW_CENTER.y + inDirect.GetNowpos().y,
			DirectX::XMFLOAT4{ 0,0,0,1 });
	}
	else if(drawingStatus == DIRECTION_OUT){
		spriteBlack.DrawExtendSprite(WINDOW_CENTER.x - outDirect.GetNowpos().x,
			WINDOW_CENTER.y - outDirect.GetNowpos().y,
			WINDOW_CENTER.x + outDirect.GetNowpos().x,
			WINDOW_CENTER.y + outDirect.GetNowpos().y,
			DirectX::XMFLOAT4{ 0,0,0,1 });
	}
	spriteBlack.Draw();
}
