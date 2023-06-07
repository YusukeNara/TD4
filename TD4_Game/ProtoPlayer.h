#pragma once

#include <vector>
#include <memory>
#include <array>
#include <Sprite.h>
#include <NY_Object3D.h>
#include <NY_Object3dMgr.h>
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

	void DrawUI();

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

	/// <summary>
	/// 退職金(HP)
	/// </summary>
	int RetirementMoney = 0;

	/// <summary>
	/// 退職金の最大値
	/// </summary>
	const int MaxRetirementMoney = 500;

private:

	RVector3 HandPositionOffset;
	RVector3 HandRotationOffset;
	RVector3 CutPositionOffset;
	RVector3 CutRotationOffset;
	RVector3 ClipPositionOffset;
	RVector3 ClipRotationOffset;
	int enemyType = SkinHead;
	ItemType handItemType = Hand;

	std::unique_ptr<Object3d> handObject = nullptr;
	std::unique_ptr<Object3d> barikanObject = nullptr;
	std::unique_ptr<Object3d> scissorsObject = nullptr;

	UINT modelPlayer;
	UINT modelScissor;

	UINT UITexHand;
	UINT UITexClip;
	UINT UITexScis;
	Sprite handUI;
	Sprite clipUI;
	Sprite scisUI;

	XMFLOAT2 uiOffsetHand;
	XMFLOAT2 uiOffsetClip;
	XMFLOAT2 uiOffsetScis;
};

