#pragma once
#include "Head.h"

#include <NY_Object3dMgr.h>
#include <memory>

class HageHead : public Head
{
public:
	HageHead();
	~HageHead();

	void Init() override;

	void ResetFrontEase() override;

	void Update() override;

	void Draw() override;

	void DrawParticle() override;

	void Finalize() override;

	void KramerMove() override;

	void SlappingMove() override;

	void FailMove() override;

	void GoHome() override;

private:

	std::shared_ptr<Object3d> headObjectSelf;

	//位置のオフセット
	RVector3 headOffset;
};

