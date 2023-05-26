#include "HageHead.h"
#include<NY_random.h>

HageHead::HageHead()
{
	headOffset = RVector3(0, 10.f, 0);
}

HageHead::~HageHead()
{
}

void HageHead::Init()
{
	SlapParticle = std::make_unique<ParticleManager>();
	SlapParticle.reset(ParticleManager::Create());
	slapTex = TexManager::LoadTexture("Resources/white1x1.png");

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
	FrontEnd = { FrontStart.x,FrontStart.y,FrontStart.z - 100.0f };
	isFrontEase = true;
}

void HageHead::Update()
{
	headObject->SetAffineParamTranslate(pos + headOffset);

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

	if (isactive)
	{
		if (!isMostFront)
		{
			return;
		}
		KramerMove();

		SlappingMove();
	}

	SlapParticle->Update();
}

void HageHead::Draw()
{
	headObject->DrawObject();
	SlapParticle->Draw(slapTex);
}

void HageHead::Finalize()
{
}

void HageHead::KramerMove()
{
	if (!isKramer)
	{
		return;
	}

	AngreeTime++;
	//怒ってるアニメーション

	if (AngreeTime >= MaxAngreeTime)
	{
		//反撃アニメーションをして、退職金を減らす

		playerPtr->RetirementMoney -= 30;
	}
}

void HageHead::SlappingMove()
{
	if (!isHairDestroy && !isKramer)
	{
		return;
	}

	if (playerPtr->GetItemType() != ItemType::Hand)
	{
		return;
	}

	//プレイヤーの入力を受け付けたら
	//アニメーションしてふっとんっでいく処理
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
			pos.x -= 0.2f;
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

void HageHead::FailMove()
{
	playerPtr->RetirementMoney -= 20;
}