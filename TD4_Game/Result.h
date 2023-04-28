#pragma once

#include <vector>
#include <memory>
#include <array>
#include <NY_Object3dMgr.h>

#include "ProtoPlayer.h"
class Result
{
public:

	Result();
	~Result();

	void Init();

	void Update();

	void Draw();

	void UIDraw();

	void Finalize();

private:

	std::shared_ptr<Object3d> groundObject;

	UINT groundTex;
};

