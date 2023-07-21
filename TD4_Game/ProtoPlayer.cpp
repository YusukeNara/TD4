#include "ProtoPlayer.h"

ProtoPlayer::ProtoPlayer()
{

	modelPlayer = TexManager::LoadTexture("Resources/blackParticleTex.png");

	UITexHand = TexManager::LoadTexture("Resources/hand.png");
	UITexClip = TexManager::LoadTexture("Resources/pull.png");
	UITexScis = TexManager::LoadTexture("Resources/cut.png");

	handObject = std::make_unique<Object3d>();
	handObject.reset(LoadModel_FBXFile("HAND"));
	barikanObject = std::make_unique<Object3d>();
	barikanObject.reset(LoadModel_FBXFile("barikan"));
	scissorsObject = std::make_unique<Object3d>();
	scissorsObject.reset(LoadModel_FBXFile("scissors"));

	handUI.Create(UITexHand);
	clipUI.Create(UITexClip);
	scisUI.Create(UITexScis);

	slapSE = std::make_unique<SoundData>();
	slapSE.reset(Audio::LoadSound_wav("Resources/slap1.wav"));
	cutSE = std::make_unique<SoundData>();
	cutSE.reset(Audio::LoadSound_wav("Resources/cut1.wav"));
	clipSE = std::make_unique<SoundData>();
	clipSE.reset(Audio::LoadSound_wav("Resources/pull.wav"));
}

ProtoPlayer::~ProtoPlayer()
{
}

void ProtoPlayer::Init()
{
	//手のアフィン変換
	HandPositionOffset = { -35,0,-50 };
	HandRotationOffset = { 90,0,0 };
	HandScaleOffset = { 0.08,0.08,0.08 };

	//バリカンのアフィン変換
	CutPositionOffset = { 0,0,-50 };
	CutRotationOffset = { 0,-90,-60 };
	CutScaleOffset = { 0.08,0.08,0.08 };

	//ハサミのアフィン変換
	ClipPositionOffset = { 35,0,-50 };
	ClipRotationOffset = { 45,0,0 };
	ClipScaleOffset = { 0.01,0.01,0.01 };

	handObject->SetAffineParam(HandScaleOffset, HandRotationOffset, HandPositionOffset);
	barikanObject->SetAffineParam(CutScaleOffset, CutRotationOffset, CutPositionOffset);
	scissorsObject->SetAffineParam(ClipScaleOffset, ClipRotationOffset, ClipPositionOffset);

	uiOffsetHand = { 50,600 };
	uiOffsetScis = { 170,600 };
	uiOffsetClip = { 290,600 };

	RetirementMoney = MaxRetirementMoney;

	//----------------制御点の座標---------------
	//ビンタ
	SlapControlPoint[1] = HandPositionOffset;
	SlapControlPoint[2] = RVector3(-50, 0, -10);
	SlapControlPoint[3] = RVector3(-15, 0, -15);
	SlapControlPoint[4] = HandPositionOffset;
	SlapControlPoint[0] = SlapControlPoint[4];
	SlapControlPoint[5] = SlapControlPoint[1];
	SlapSpline.Init(SlapControlPoint.data(), 6, 15);

	//バリカン
	CutControlPoint[1] = CutPositionOffset;
	CutControlPoint[2] = RVector3(  0, 0, -10);
	CutControlPoint[3] = RVector3(  0,40, -30);
	CutControlPoint[4] = CutPositionOffset;
	CutControlPoint[0] = CutControlPoint[4];
	CutControlPoint[5] = CutControlPoint[1];
	CutSpline.Init(CutControlPoint.data(), 6, 15);

	//ハサミ
	ClipControlPoint[1] = ClipPositionOffset;
	ClipControlPoint[2] = RVector3( 50, 0, -10);
	ClipControlPoint[3] = RVector3( 15, 0, -15);
	ClipControlPoint[4] = ClipPositionOffset;
	ClipControlPoint[0] = ClipControlPoint[4];
	ClipControlPoint[5] = ClipControlPoint[1];
	ClipSpline.Init(ClipControlPoint.data(), 6, 15);
}

void ProtoPlayer::Update()
{
	handObject->SetAffineParam(HandScaleOffset, HandRotationOffset, HandPositionOffset);
	barikanObject->SetAffineParam(CutScaleOffset, CutRotationOffset, CutPositionOffset);
	scissorsObject->SetAffineParam(ClipScaleOffset, ClipRotationOffset, ClipPositionOffset);



	Attack();

	ChangeItem();
}

void ProtoPlayer::Draw()
{
	handObject->DrawObject();
	barikanObject->DrawObject();
	scissorsObject->DrawObject();
}

void ProtoPlayer::DrawUI()
{
	handUI.DrawExtendSprite(uiOffsetHand.x, uiOffsetHand.y, uiOffsetHand.x + 100, uiOffsetHand.y + 100);
	handUI.Draw();
	clipUI.DrawExtendSprite(uiOffsetClip.x, uiOffsetClip.y, uiOffsetClip.x + 100, uiOffsetClip.y + 100);
	clipUI.Draw();
	scisUI.DrawExtendSprite(uiOffsetScis.x, uiOffsetScis.y, uiOffsetScis.x + 100, uiOffsetScis.y + 100);
	scisUI.Draw();
}

void ProtoPlayer::Attack()
{
	HandAttack();

	CutHair();

	Clip();
}

void ProtoPlayer::Finalize()
{
}

void ProtoPlayer::HandAttack()
{
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_X) || Input::isKeyTrigger(DIK_LEFT))
	{
		if (isSlapSpline)
		{
			SlapSpline.Reset();
		}
		if (isCutSpline)
		{
			CutSpline.Reset();
		}
		if (isClipSpline)
		{
			ClipSpline.Reset();
		}
		SlapSpline.Play();
		SlapRot.y = -20;
		isSlapSpline = true;
	}

	if (!isSlapSpline)
	{
		handObject->SetAffineParamRotate(HandRotationOffset);
		handObject->SetAffineParamTranslate(HandPositionOffset);
		return;
	}

	if (isSlapSpline)
	{
		//SlapRot.y += 3;
		handObject->SetAffineParamRotate(SlapRot);
		handObject->SetAffineParamTranslate(SlapSpline.Update());

		if (handObject->position == SlapControlPoint[2])
		{
			SlapRot.y = -10;
		}
		else if (handObject->position == SlapControlPoint[3])
		{
			SlapRot.y = 40;
		}

		if (handObject->position == HandPositionOffset)
		{
			SlapSpline.Reset();
			isSlapSpline = false;
		}
	}
}

void ProtoPlayer::CutHair()
{
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_Y) || Input::isKeyTrigger(DIK_UP))
	{
		if (isSlapSpline)
		{
			SlapSpline.Reset();
		}
		if (isCutSpline)
		{
			CutSpline.Reset();
		}
		if (isClipSpline)
		{
			ClipSpline.Reset();
		}
		CutSpline.Play();
		CutRot.z = -20;
		isCutSpline = true;
	}

	if (!isCutSpline)
	{
		barikanObject->SetAffineParamRotate(CutRotationOffset);
		barikanObject->SetAffineParamTranslate(CutPositionOffset);
		return;
	}

	if (isCutSpline)
	{
		//SlapRot.y += 3;
		//barikanObject->SetAffineParamRotate(CutRot);
		barikanObject->SetAffineParamTranslate(CutSpline.Update());

		if (barikanObject->position == CutControlPoint[2])
		{
			CutRot.y = -10;
		}
		else if (barikanObject->position == CutControlPoint[3])
		{
			CutRot.y = 40;
		}

		if (barikanObject->position == CutPositionOffset)
		{
			CutSpline.Reset();
			isCutSpline = false;
		}
	}
}

void ProtoPlayer::Clip()
{
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_B) || Input::isKeyTrigger(DIK_RIGHT))
	{
		if (isSlapSpline)
		{
			SlapSpline.Reset();
		}
		if (isCutSpline)
		{
			CutSpline.Reset();
		}
		if (isClipSpline)
		{
			ClipSpline.Reset();
		}
		ClipSpline.Play();
		ClipRot.y = -20;
		isClipSpline = true;
	}

	if (!isClipSpline)
	{
		scissorsObject->SetAffineParamRotate(ClipRotationOffset);
		scissorsObject->SetAffineParamTranslate(ClipPositionOffset);
		return;
	}

	if (isClipSpline)
	{
		//SlapRot.y += 3;
		//scissorsObject->SetAffineParamRotate(ClipRot);
		scissorsObject->SetAffineParamTranslate(ClipSpline.Update());

		if (scissorsObject->position == ClipControlPoint[2])
		{
			ClipRot.y = -10;
		}
		else if (scissorsObject->position == ClipControlPoint[3])
		{
			ClipRot.y = 40;
		}

		if (scissorsObject->position == ClipPositionOffset)
		{
			ClipSpline.Reset();
			isClipSpline = false;
		}
	}
}

void ProtoPlayer::ChangeItem()
{
	//アイテムの切り替え

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_X) || Input::isKeyTrigger(DIK_LEFT))
	{
		handItemType = Hand;
	}

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_Y) || Input::isKeyTrigger(DIK_UP))
	{
		handItemType = Scissors;
	}

	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_B) || Input::isKeyTrigger(DIK_RIGHT))
	{
		handItemType = Clippers;
	}

	//--------------------UI---------------------
	if (handItemType == Hand)
	{
		uiOffsetHand.y = 570;//
		uiOffsetClip.y = 600;
		uiOffsetScis.y = 600;
	}
	else if (handItemType == Clippers)
	{
		uiOffsetHand.y = 600;
		uiOffsetClip.y = 570;//
		uiOffsetScis.y = 600;
	}
	else
	{
		uiOffsetHand.y = 600;
		uiOffsetClip.y = 600;
		uiOffsetScis.y = 570;//
	}
}

ItemType ProtoPlayer::GetItemType()
{
	return handItemType;
}
