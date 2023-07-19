#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "FbxLoader.h"
#include <DirectXMath.h>
#include <cmath>

#include <Raki_Input.h>
#include <NY_Object3DMgr.h>
#include <Sprite.h>

class SceneChangeDirection
{
	SceneChangeDirection();

	~SceneChangeDirection();

	//演出初期化
	void Init();
	//シーン開始演出開始
	void PlayInDirection();
	//シーン終了演出開始
	void PlayOutDirection();
	//更新（毎フレーム実行）
	void Update();
	//描画
	void Draw();

private:
	//演出に必要な変数はここ

	Sprite spriteBlack;
	XMFLOAT2 spritePos;
	XMFLOAT2 spriteSize;
	const int animationCountMax = 180;
	int animationCount = 0;
	bool isAnimation = false;
};

