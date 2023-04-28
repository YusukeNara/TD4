#pragma once
#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>

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
	//タイトルロゴとかのリソース
};