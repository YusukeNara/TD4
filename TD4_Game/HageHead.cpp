#include "HageHead.h"

HageHead::HageHead()
{
	headOffset = RVector3(0, 10.f, 0);
}

HageHead::~HageHead()
{
}

void HageHead::Init()
{
	hageheadTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	headObject = std::make_shared<Object3d>();

	headObject.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, hageheadTex));

	scale = RVector3(1, 1, 1);
	rot = RVector3(0, 0, 0);
	pos.zero();
	headObject->SetAffineParam(scale, rot, pos);
	isHairDestroy = true;
	SlapCount = 0;
	isKramer = false;
	isactive = false;
	ResetFrontEase();
}

void HageHead::ResetFrontEase()
{
	FrontStart = pos;
	FrontEnd = { FrontStart.x,FrontStart.y,FrontStart.z - FrontLength };
	isFrontEase = true;
}

void HageHead::Update()
{
	headObject->SetAffineParam(scale, rot, pos);

	if (isFrontEase)
	{
		if (pos.z <= FrontEnd.z)
		{
			isactive = true;
			isFrontEase = false;
		}
		else
		{
			pos = Rv3Ease::OutQuad(FrontStart, FrontEnd, (float)FrontEaseT);
			FrontEaseT++;
		}
	}

	if (isactive)
	{
		if (!isMostFront)
		{
			//return;
		}
		SlappingMove();
	}
}

void HageHead::Draw()
{
	headObject->DrawObject();
}

void HageHead::Finalize()
{
}

void HageHead::SlappingMove()
{
	if (!isHairDestroy)
	{
		return;
	}

	//プレイヤーの入力を受け付けたら
	//if(ptr->)
	//{}

	//アニメーションしてふっとんっでいく処理

	if (isSlap)
	{
		if (isKramer)
		{
			SlapCount++;
			if (SlapCount >= 3)
			{
				isAllMoveFinish = true;
			}
			isSlap = false;
		}
		else
		{
			pos.x -= 0.5f;
			if (pos.x < -3)
			{
				isAllMoveFinish = true;
			}
		}
	}

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		isSlap = true;
	}

}
