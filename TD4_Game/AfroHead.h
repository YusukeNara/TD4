#pragma once
#include "Head.h"
#include "ProtoPlayer.h"

#include <NY_Object3dMgr.h>
#include <memory>

class AfroHead : public Head
{
public:
	AfroHead();
	~AfroHead();

	void Init() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void HairCut() override;

	void Slap() override;

private:

	std::shared_ptr<Object3d> headObject;
	std::shared_ptr<Object3d> afroObject;

	//位置のオフセット
	RVector3 headOffset;
	RVector3 hearOffset;

	UINT afroheadTex;

	int hairHP = 5;

};

