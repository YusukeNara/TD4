#include "ProtoPlayer.h"

ProtoPlayer::ProtoPlayer()
{
	player = std::make_shared<Object3d>();
}

ProtoPlayer::~ProtoPlayer()
{
}

void ProtoPlayer::Init()
{
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 0,0,0 };
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
	if (enemyType == SkinHead)
	{
		handItemType = Hand;
	}
	if (enemyType == Thinning)
	{
		handItemType = Scissors;
	}
	if (enemyType == Afro)
	{
		handItemType = Clippers;
	}
}

ItemType ProtoPlayer::GetItemType()
{
	return ItemType();
}
