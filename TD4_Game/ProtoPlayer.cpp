#include "ProtoPlayer.h"

ProtoPlayer::ProtoPlayer()
{
}

ProtoPlayer::~ProtoPlayer()
{
}

void ProtoPlayer::Init()
{
}

void ProtoPlayer::Update(std::shared_ptr<Head> head)
{

	if (Input::Get()->isKeyTrigger(DIK_RIGHT)) { Slap(head); }
	if (Input::Get()->isKey(DIK_LEFT)) { Cut(head); }

}

void ProtoPlayer::Draw()
{
}

void ProtoPlayer::UIDraw()
{
}

void ProtoPlayer::Cut(std::shared_ptr<Head> head)
{
	//頭に散髪を実行
	head->HairCut();
}

void ProtoPlayer::Slap(std::shared_ptr<Head> head)
{
	//頭にビンタを実行
	head->Slap();
}
