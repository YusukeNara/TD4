#pragma once
#include "Head.h"

#include <NY_Object3dMgr.h>
#include <memory>

class AfroHead : public Head
{
public:
	AfroHead();
	~AfroHead();

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
	void CuttingHair();

private:
	//毛を刈った時のパーティクル
	std::unique_ptr<ParticleManager> CutParticle;
	UINT cutTex;

	std::shared_ptr<Object3d> headObjectSelf;
	std::shared_ptr<Object3d> afroObjectSelf;

	//位置のオフセット
	RVector3 headOffset;
	RVector3 hairOffset;

	UINT afroheadTex;

	RVector3 AfroSize = {};

	//髪を斬った回数
	int CutCount = 0;
	const int MaxCutCount = 3;

};

