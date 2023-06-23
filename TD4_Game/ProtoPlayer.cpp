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
	ClipRotationOffset = { 0,0,0 };
	ClipScaleOffset = { 0.02,0.02,0.02 };

	handObject->SetAffineParam(HandScaleOffset, HandRotationOffset, HandPositionOffset);
	barikanObject->SetAffineParam(CutScaleOffset, CutRotationOffset, CutPositionOffset);
	scissorsObject->SetAffineParam(ClipScaleOffset, ClipRotationOffset, ClipPositionOffset);

	uiOffsetHand = { 50,600 };
	uiOffsetScis = { 170,600 };
	uiOffsetClip = { 290,600 };

	RetirementMoney = MaxRetirementMoney;
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
	//handObject->DrawObject();
	//barikanObject->DrawObject();
	//scissorsObject->DrawObject();
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
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A))
	{
		if (handItemType == Hand)
		{
			HandAttack();
		}
		else if (handItemType == Scissors)
		{
			CutHair();
		}
		else if (handItemType == Clippers)
		{
			Clip();
		}
	}
}

void ProtoPlayer::Finalize()
{
}

void ProtoPlayer::HandAttack()
{
}

void ProtoPlayer::CutHair()
{
}

void ProtoPlayer::Clip()
{
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
