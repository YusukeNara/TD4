#include "LightHearHead.h"

#include<NY_random.h>

LightHairHead::LightHairHead()
{
	headOffset = RVector3(0, 10.f, 0);
	hairOffset = RVector3(0, 20.0f, 0);
}

LightHairHead::~LightHairHead()
{
}

void LightHairHead::Init()
{
	SlapParticle = std::make_unique<ParticleManager>();
	SlapParticle.reset(ParticleManager::Create());
	slapTex = TexManager::LoadTexture("Resources/white1x1.png");

	PullParticle = std::make_unique<ParticleManager>();
	PullParticle.reset(ParticleManager::Create());
	pullTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	lighthairTex = TexManager::LoadTexture("Resources/blackParticleTex.png");

	headObject = std::make_shared<Object3d>();
	hairObject = std::make_shared<Object3d>();

	headObject.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, lighthairTex));
	hairObject.reset(NY_Object3DManager::Get()->CreateModel_Box(10.f, 1.f, 1.f, lighthairTex));

	scale = RVector3(1, 1, 1);
	rot = RVector3(0, 0, 0);
	pos.zero();
	headObject->SetAffineParam(scale, rot, pos);
	hairObject->SetAffineParam({ 0.2,0.5,0.2 }, rot, pos);
	SlapCount = 0;
	isKramer = false;
	isactive = false;
	ResetFrontEase();
}

void LightHairHead::ResetFrontEase()
{
	FrontStart = pos;
	FrontEnd = { FrontStart.x,FrontStart.y,FrontStart.z - 100.0f };
	isFrontEase = true;
}

void LightHairHead::Update()
{
	//オブジェクト描画位置を設定
	headObject->SetAffineParamTranslate(pos + headOffset);
	hairObject->SetAffineParamTranslate(pos + hairOffset);

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

		PullOutHair();

		FailMove();
	}

	SlapParticle->Update();
	PullParticle->Update();
}

void LightHairHead::Draw()
{
	//オブジェクト描画
	if (isKramer)
	{
		headObject->DrawObject();
		hairObject->DrawObject();
	}
	else
	{
		headObject->DrawObject();
		if (!isHairDestroy)
		{
			hairObject->DrawObject();
		}
	}
	SlapParticle->Draw(slapTex);
	PullParticle->Draw(pullTex);
}

void LightHairHead::Finalize()
{
}

void LightHairHead::KramerMove()
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

void LightHairHead::SlappingMove()
{
	if (!isHairDestroy && !isKramer || isGoHome || isFail)
	{
		return;
	}

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
		if (playerPtr->GetItemType() != ItemType::Hand)
		{
			isFail = true;
			return;
		}

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

void LightHairHead::FailMove()
{
	if (!isFail)
	{
		return;
	}
	playerPtr->RetirementMoney -= 20;
	isFail = false;
}

void LightHairHead::GoHome()
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

void LightHairHead::PullOutHair()
{
	if (isHairDestroy || isKramer || isGoHome || isFail || isCounter)
	{
		return;
	}

	//プレイヤーの入力を受け付けたら
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		if (playerPtr->GetItemType() != ItemType::Clippers)
		{
			isFail = true;
			return;
		}

		isHairDestroy = true;

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

			PullParticle->Add(pgstate);
		}
	}
}

void LightHairHead::CounterMove()
{
	if (!isCounter)
	{
		return;
	}

	AngreeTime++;

	if (AngreeTime >= MaxAngreeTime)
	{
		//逆ギレして帰る
		isGoHome = true;
	}
}
