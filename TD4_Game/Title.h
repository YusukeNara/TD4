#pragma once
#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>
#include <Sprite.h>

#include "Head.h"
#include "ProtoPlayer.h"

class Title
{
public:
	Title();
	~Title();

	void Init();

	void Update();

	void Draw();

	void UIDraw();

	void Finalize();

private:
	//3D素材
	//タイトル背景
	Object3d titleBack3D;

	//2D素材
	//タイトル背景
	Sprite titleBack2D;
	//タイトルロゴ
	Sprite titleLogo;
	//スタートボタンUI
	Sprite startButtonUI;

	//変数

};