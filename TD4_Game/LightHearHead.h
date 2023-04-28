#pragma once
#include "Head.h"

#include <NY_Object3dMgr.h>
#include <memory>

class LightHairHead : public Head
{
public:
	LightHairHead();
	~LightHairHead();

	void Init() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void SlappingMove() override;

private:
	void PullOutHair();

private:

	std::shared_ptr<Object3d> headObject;
	std::shared_ptr<Object3d> hairObject;

	//位置のオフセット
	RVector3 headOffset;
	RVector3 hairOffset;

	UINT lighthairTex;
};

