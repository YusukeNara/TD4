#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "FbxLoader.h"
#include <DirectXMath.h>
#include <cmath>

#include <Raki_Input.h>
#include <NY_Object3DMgr.h>
#include <Sprite.h>

enum DIRECTION_STATUS
{
	DIRECTION_STANDBY,
	DIRECTION_IN,
	DIRECTION_OUT,
	DIRECTION_ENDED,
};

class SceneChangeDirection
{
public:
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

	DIRECTION_STATUS GetDirectionStatus() { return status; }

private:
	//演出に必要な変数はここ

	Sprite spriteBlack;
	XMFLOAT2 spritePos;
	XMFLOAT2 spriteSize;
	const int animationCountMax = 60;
	int animationCount = 0;
	bool isAnimation = false;

	DIRECTION_STATUS status;
	DIRECTION_STATUS drawingStatus;

	const RVector3 WINDOW_CENTER = RVector3(640, 360, 0);

	const float EASE_MOVE_X_MAX = 640.f;
	const float EASE_MOVE_Y_MAX = 360.f;


	Rv3Ease::Rv3Easing inDirect, outDirect;
};

