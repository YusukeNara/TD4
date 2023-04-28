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
}

void ProtoPlayer::Draw()
{
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

void ProtoPlayer::ChangeItem()
{
}
