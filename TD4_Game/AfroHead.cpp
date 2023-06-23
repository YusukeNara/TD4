#include "AfroHead.h"

#include <Raki_Input.h>
#include<NY_random.h>

AfroHead::AfroHead()
{
	headOffset = RVector3(0, 10.f, 0);
	hairOffset = RVector3(0, 13.0f, 0);
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

	headObjectSelf = std::make_shared<Object3d>();
	afroObjectSelf = std::make_shared<Object3d>();

	headObjectSelf.reset(LoadModel_FBXFile("hage_1"));
	afroObjectSelf.reset(LoadModel_FBXFile("kamihusahusa"));

	scale = RVector3(0.1, 0.1, 0.1);
	rot = RVector3(0, 90, 0);
	pos.zero();
	headObjectSelf->SetAffineParam(scale, rot, pos);
	afroObjectSelf->SetAffineParam({ 0.101,0.101,0.101 }, rot, pos);
	SlapCount = 0;
	AfroSize = afroObjectSelf->scale / 8;
	isKramer = false;
	isactive = false;
	HeadType = CheraType::Afro;
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
	headObjectSelf->SetAffineParamTranslate(pos + headOffset);
	afroObjectSelf->SetAffineParamTranslate(pos + hairOffset);

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

		FailMove();
	}

	SlapParticle->Update();
	CutParticle->Update();
}

void AfroHead::Draw()
{
	//オブジェクト描画
	if (isKramer)
	{
		headObjectSelf->DrawObject();
		afroObjectSelf->DrawObject();
	}
	else
	{
		headObjectSelf->DrawObject();
		if (!isHairDestroy)
		{
			afroObjectSelf->DrawObject();
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

	//怒ってるアニメーション
	if (headObjectSelf->position.y < 4 || headObjectSelf->position.y > 10)
	{
		positionUpDown *= -1;
	}

	pos.y += positionUpDown * 1.5f;

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

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_B) ||
		Input::isXpadButtonPushTrigger(XPAD_BUTTON_Y) ||
		Input::isKeyTrigger(DIK_UP) ||
		Input::isKeyTrigger(DIK_RIGHT))
	{
		isFail = true;
		ShakeBacePos = pos;
		pos.x = pos.x + ShakeOffset;
		FailCount = 0;
		return;
	}

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_X) || Input::isKeyTrigger(DIK_LEFT))
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
			pgstate.scale_start = 3.0f;
			pgstate.scale_end = 4.5f;
			pgstate.aliveTime = 60;

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

	FailCount++;

	if (FailCount % 2 == 0)
	{
		ShakeOffset *= -1;
		pos.x += ShakeBacePos.x + (ShakeOffset * 2);
	}

	if (FailCount >= 20)
	{
		pos = ShakeBacePos;
		playerPtr->RetirementMoney -= 20;
		isFail = false;
	}
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

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_X) || 
		Input::isXpadButtonPushTrigger(XPAD_BUTTON_B) || 
		Input::isKeyTrigger(DIK_LEFT) || 
		Input::isKeyTrigger(DIK_RIGHT))
	{
		isFail = true;
		ShakeBacePos = pos;
		pos.x = pos.x + ShakeOffset;
		FailCount = 0;
		return;
	}

	//プレイヤーの入力を受け付けたら
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_Y) || Input::isKeyTrigger(DIK_UP))
	{
		CutCount++;

		//パーティクル生成
		for (int i = 0; i < 40; i++)
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
			pgstate.aliveTime = 40;

			CutParticle->Add(pgstate);
		}

		afroObjectSelf->scale -= AfroSize;
	}


	if (CutCount >= MaxCutCount)
	{
		isHairDestroy = true;
		HeadType = CheraType::SkinHead;
	}
}
