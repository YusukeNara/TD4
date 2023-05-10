#pragma once

#include <vector>
#include <memory>
#include <array>
#include <NY_Object3D.h>
#include "Raki_Input.h"


enum CheraType
{
	None, SkinHead, Thinning, Afro,
};
enum ItemType
{
	Hand, Scissors, Clippers
};

class ProtoPlayer
{
public:
	ProtoPlayer();
	~ProtoPlayer();

	void Init();

	void Update();

	void Draw();

	void Attack();

	void Finalize();

	//ビンタ
	void HandAttack();

	//髪を切る
	void CutHair();

	//バリカン
	void Clip();

	//左手のアイテムを切り替える
	void ChangeItem();

	ItemType GetItemType();

private:
	
	RVector3 position;
	RVector3 rotation;
	RVector3 scale;
	int enemyType = SkinHead;
	int handItemType = Hand;

	std::shared_ptr<Object3d> player = nullptr;
	std::shared_ptr<Object3d> scissor = nullptr;

	UINT modelPlayer;
};

