#pragma once
#include <RVector.h>
#include<ParticleManager.h>

#include "ProtoPlayer.h"

using namespace Rv3Ease;

class Head
{
public:

	Head() {};
	virtual ~Head() {};

	virtual void Init() {};

	virtual void ResetFrontEase() {};

	virtual void Update() {};

	virtual void Draw() {};

	virtual void Finalize() {};

	virtual void SlappingMove() {};

	void SetPlayer(std::shared_ptr<ProtoPlayer> ptr) {
		this->ptr = ptr;
	}

	void Activate() {
		isactive = true;
	}

public:
	//ビンタされた時のパーティクル
	std::unique_ptr<ParticleManager> SlapParticle;
	UINT slapTex;

	//アフィン変換
	RVector3 pos, rot, scale;

	//客のタイプ
	CheraType HeadType = CheraType::None;

	//先頭かどうか
	bool isMostFront = false;

	//有効化フラグ
	bool isactive = false;

	//待機時間
	int waitTime = 0;

	//髪を切られた・抜かれたかどうか
	bool isHairDestroy = false;

	//怒っているかどうか
	bool isAngree = false;

	//怒っている時間
	int AngreeTime = 0;

	//反撃するかどうか
	bool isCounter = false;

	//処理が終わったかどうか
	bool isAllMoveFinish = false;

	//プレイヤーポインタ
	std::weak_ptr<ProtoPlayer> ptr;

	//クレーマーかどうか
	bool isKramer = false;

	//ビンタフラグ
	bool isSlap = false;

	//ビンタされた回数
	int SlapCount = 0;

	//ビンタの最大回数
	const int ManSlapCount = 7;

	//イージング用変数
	//前に進む時
	bool isFrontEase = false;		//前に進むか
	RVector3 FrontStart = {};
	RVector3 FrontEnd = {};
	int FrontEaseT = 0;		//時間
	const float FrontLength = 2.5f;	//変化量
	//ビンタされたとき
	float SlapEaseT = 0.0f;			//時間
	const float SlapLength = 1.0f;	//変化量
};

