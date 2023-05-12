#include "AfroHead.h"

#include <Raki_Input.h>

AfroHead::AfroHead()
{
	headOffset = RVector3(0, 10.f, 0);
	hairOffset = RVector3(0, 20.0f, 0);
}

AfroHead::~AfroHead()
{
}

void AfroHead::Init()
{
	afroheadTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	headObject = std::make_shared<Object3d>();
	afroObject = std::make_shared<Object3d>();

	headObject.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, afroheadTex));
	afroObject.reset(NY_Object3DManager::Get()->CreateModel_Box(10.f, 1.f, 1.f, afroheadTex));

	scale = RVector3(1, 1, 1);
	rot = RVector3(0, 0, 0);
	pos.zero();
	headObject->SetAffineParam(scale, rot, pos);
	afroObject->SetAffineParam(scale, rot, pos);
	SlapCount = 0;
	isKramer = false;
	isactive = true;
	ResetFrontEase();
}

void AfroHead::ResetFrontEase()
{
	FrontStart = pos;
	FrontEnd = { FrontStart.x,FrontStart.y,FrontStart.z - FrontLength };
	isFrontEase = true;
}

void AfroHead::Update()
{
	//オブジェクト描画位置を設定
	headObject->SetAffineParamTranslate(pos + headOffset);
	afroObject->SetAffineParamTranslate(pos + hairOffset);

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

	// 頭が有効化されたら
	if (isactive) 
	{
		if (!isMostFront)
		{
			//return;
		}

		//入力を受け付ける
		SlappingMove();

		CuttingHair();

	}


}

void AfroHead::Draw()
{
	//オブジェクト描画
	headObject->DrawObject();
	if (!isHairDestroy)
	{
		afroObject->DrawObject();
	}
}

void AfroHead::Finalize()
{



}

void AfroHead::SlappingMove()
{
	if (!isHairDestroy)
	{
		return;
	}

	//if(ptr->)
	//{}

	//プレイヤーの入力を受け付けたら
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

void AfroHead::CuttingHair()
{
	if (isHairDestroy)
	{
		return;
	}

	//プレイヤーの入力を受け付けたら
	//if(ptr->)
	//{
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		CutCount++;
	}
	//}


	if (CutCount >= MaxCutCount)
	{
		isHairDestroy = true;
	}
}
