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

	void ResetFrontEase() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	void KramerMove() override;

	void SlappingMove() override;

	void FailMove() override;

	void GoHome() override;

private:
	void PullOutHair();

	void CounterMove();

private:
	//毛を抜いた時のパーティクル
	std::unique_ptr<ParticleManager> PullParticle;
	UINT pullTex;

	std::shared_ptr<Object3d> headObject;
	std::shared_ptr<Object3d> hairObject;

	//位置のオフセット
	RVector3 headOffset;
	RVector3 hairOffset;

	UINT lighthairTex = 0;
};

