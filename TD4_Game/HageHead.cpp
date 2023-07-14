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

	headObjectSelf = std::make_shared<Object3d>();
	headObjectSelf.reset(LoadModel_FBXFile("hage_1"));

	scale = RVector3(0.1, 0.1, 0.1);
	rot = RVector3(0, 90, 0);
	pos.zero();
	headObjectSelf->SetAffineParam(scale, rot, pos);
	isHairDestroy = true;
	SlapCount = 0;
	//isKramer = false;
	isactive = false;
	HeadType = CheraType::SkinHead;
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
	headObjectSelf->SetAffineParamTranslate(pos + headOffset);
	headObjectSelf->SetAffineParamRotate(rot);

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
		waitTime++;
		if (waitTime >= MaxWaitTime)
		{
			isGoHome = true;
		}
		GoHome();

		KramerMove();

		SlappingMove();

		FailMove();
	}

	SlapParticle->Update();
}

void HageHead::Draw()
{
	headObjectSelf->DrawObject();
}

void HageHead::DrawParticle()
{
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

void HageHead::SlappingMove()
{
	if (!isHairDestroy && !isKramer || isGoHome || isFail)
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
			blustTime++;
			pos += blustVec;
			rot += blustRot;
			if (blustTime > maxBustTime)
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

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_X) || Input::isKeyTrigger(DIK_LEFT) && !isSlap)
	{
		isSlap = true;

		//飛ぶ方向を決定
		blustVec = RVector3(NY_random::floatrand_sl(30, 0), NY_random::floatrand_sl(30, 0), 0);
		blustVec = blustVec.norm() * 7;

		blustRot = RVector3(NY_random::floatrand_sl(10, 0), NY_random::floatrand_sl(10, 0), NY_random::floatrand_sl(10, 0));
		blustRot *= 5;

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

void HageHead::FailMove()
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

void HageHead::GoHome()
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
