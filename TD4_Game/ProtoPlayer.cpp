#include "ProtoPlayer.h"

ProtoPlayer::ProtoPlayer()
{

	modelPlayer = TexManager::LoadTexture("Resources/blackParticleTex.png");

	player = std::make_shared<Object3d>();

	player.reset(NY_Object3DManager::Get()->CreateModel_Box(5.f, 1.f, 1.f, modelPlayer));
}

ProtoPlayer::~ProtoPlayer()
{
}

void ProtoPlayer::Init()
{
	position = { 0,5,0 };
	rotation = { 0,0,0 };
	scale = { 3,3,3 };
	player->SetAffineParam(scale, rotation, position);
}

void ProtoPlayer::Update()
{
	Attack();
}

void ProtoPlayer::Draw()
{
	player->DrawObject();
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
	if (handItemType == Hand)
	{
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R))
		{
			handItemType = Scissors;
		}
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L))
		{
			handItemType = Clippers;
		}
	}
	else if (handItemType == Scissors)
	{
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R))
		{
			handItemType = Clippers;
		}
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L))
		{
			handItemType = Hand;
		}
	}
	else if (handItemType == Clippers)
	{
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R))
		{
			handItemType = Hand;
		}
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L))
		{
			handItemType = Scissors;
		}
	}
}

ItemType ProtoPlayer::GetItemType()
{
	return ItemType();
}
