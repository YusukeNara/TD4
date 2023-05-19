#pragma once

#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>

#include "ProtoPlayer.h"
#include"BaseScene.h"
#include"ScoreManager.h"

class Result : public BaseScene
{
public:

	Result(ISceneChanger *changer);
	~Result();

	void Initialize() override;    //初期化処理をオーバーライド。
	void Finalize() override;   //終了処理をオーバーライド。
	void Update() override;        //更新処理をオーバーライド。
	void Draw() override;          //描画処理をオーバーライド。
	void Draw2D() override;
private:

	std::shared_ptr<Object3d> groundObject;

	UINT groundTex;
};

