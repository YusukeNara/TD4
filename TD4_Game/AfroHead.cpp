#include "AfroHead.h"

#include <Raki_Input.h>
#include<NY_random.h>

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
	SlapParticle = std::make_unique<ParticleManager>();
	SlapParticle.reset(ParticleManager::Create());
	slapTex = TexManager::LoadTexture("Resources/white1x1.png");

	CutParticle = std::make_unique<ParticleManager>();
	CutParticle.reset(ParticleManager::Create());
	cutTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	afroheadTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	headObject = std::make_shared<Object3d>();
	afroObject = std::make_shared<Object3d>();

	headObject.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, afroheadTex));
	afroObject.reset(NY_Object3DManager::Get()->CreateModel_Box(10.f, 1.f, 1.f, afroheadTex));

	scale = RVector3(1, 1, 1);
	rot = RVector3(0, 0, 0);
	pos.zero();
	headObject->SetAffineParam(scale, rot, pos);
	afroObject->SetAffineParam({ 1.1,1.2,1.1 }, rot, pos);
	SlapCount = 0;
	AfroSize = afroObject->scale / 4;
	isKramer = false;
	isactive = false;
	ResetFrontEase();
}

void AfroHead::ResetFrontEase()
{
	FrontStart = pos;
	FrontEnd = { FrontStart.x,FrontStart.y,FrontStart.z - 100.0f };
	isFrontEase = true;
}

void AfroHead::Update()
{
	//オブジェクト描画位置を設定
	headObject->SetAffineParamTranslate(pos + headOffset);
	afroObject->SetAffineParamTranslate(pos + hairOffset);

	if (isMostFront && !isFrontEase)
	{
		isactive = true;
	}

	if (isFrontEase && !isactive)
	{
		if (pos.z <= FrontEnd.z)
		{
			pos.z = FrontEnd.z;
			isFrontEase = false;
		}
		else
		{
			pos.z -= FrontLength;
		}
	}

	// 頭が有効化されたら
	if (isactive)
	{
		if (!isMostFront)
		{
			return;
		}
		waitTime++;
		if (waitTime >= MaxWaitTime)
		{
			isGoHome = true;
		}
		GoHome();

		KramerMove();

		//入力を受け付ける
		SlappingMove();

		CuttingHair();
	}

	SlapParticle->Update();
	CutParticle->Update();
}

void AfroHead::Draw()
{
	//オブジェクト描画
	if (isKramer)
	{
		headObject->DrawObject();
		afroObject->DrawObject();
	}
	else
	{
		headObject->DrawObject();
		if (!isHairDestroy)
		{
			afroObject->DrawObject();
		}
	}
	SlapParticle->Draw(slapTex);
	CutParticle->Draw(cutTex);
}

void AfroHead::Finalize()
{



}

void AfroHead::KramerMove()
{
	if (!isKramer)
	{
		return;
	}

	AngreeTime++;

	if (AngreeTime >= MaxAngreeTime)
	{
		//反撃アニメーションをして、退職金を減らす

		playerPtr->RetirementMoney -= 30;

		isGoHome = true;
	}
}

void AfroHead::SlappingMove()
{
	if (!isHairDestroy && !isKramer || isGoHome || isFail)
	{
		return;
	}

	if (playerPtr->GetItemType() != ItemType::Hand)
	{
		isFail = true;
		return;
	}

	//プレイヤーの入力を受け付けたら
	if (isSlap)
	{
		//クレーマーなら
		if (isKramer)
		{
			SlapCount++;
			if (SlapCount >= ManSlapCount)
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

		//パーティクル生成
		for (int i = 0; i < 30; i++)
		{
			RVector3 v(NY_random::floatrand_sl(30, -30), NY_random::floatrand_sl(30, -30), NY_random::floatrand_sl(30, -30));
			v = v.norm();

			//設定構造体のインスタンス
			ParticleGrainState pgstate{};
			//パラメータ設定
			pgstate.position = RVector3(pos.x + 5, pos.y, pos.z);
			pgstate.position = RVector3(pos.x + 5, 0, 0);
			pgstate.vel = v * 4.0f;
			pgstate.acc = -(v / 10);
			pgstate.color_start = XMFLOAT4(1, 0, 0, 1);
			pgstate.color_end = XMFLOAT4(1, 0, 0, 1);
			pgstate.scale_start = 2.0f;
			pgstate.scale_end = 2.5f;
			pgstate.aliveTime = 20;

			SlapParticle->Add(pgstate);
		}
	}
}

void AfroHead::FailMove()
{
	if (!isFail)
	{
		return;
	}
	playerPtr->RetirementMoney -= 20;
	isFail = false;
}

void AfroHead::GoHome()
{
	if (!isGoHome)
	{
		return;
	}

	pos.x += 1.0;

	if (pos.x >= 10)
	{
		isAllMoveFinish = true;
	}
}

void AfroHead::CuttingHair()
{
	if (isHairDestroy || isKramer || isGoHome || isFail)
	{
		return;
	}

	if (playerPtr->GetItemType() != ItemType::Scissors)
	{
		isFail = true;
		return;
	}

	//プレイヤーの入力を受け付けたら
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		CutCount++;

		//パーティクル生成
		for (int i = 0; i < 30; i++)
		{
			RVector3 v(NY_random::floatrand_sl(30, -30), NY_random::floatrand_sl(30, -30), NY_random::floatrand_sl(30, -30));
			v = v.norm();

			//設定構造体のインスタンス
			ParticleGrainState pgstate{};
			//パラメータ設定
			pgstate.position = RVector3(pos.x, pos.y + 5, pos.z);
			pgstate.vel = v * 4.0f;
			pgstate.acc = -(v / 10);
			pgstate.color_start = XMFLOAT4(0, 0, 0, 1);
			pgstate.color_end = XMFLOAT4(0, 0, 0, 1);
			pgstate.scale_start = 2.0f;
			pgstate.scale_end = 2.5f;
			pgstate.aliveTime = 20;

			CutParticle->Add(pgstate);
		}

		afroObject->scale -= AfroSize;
	}


	if (CutCount >= MaxCutCount)
	{
		isHairDestroy = true;
	}
}
