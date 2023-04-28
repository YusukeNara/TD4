#pragma once
#include <RVector.h>

#include "ProtoPlayer.h"


class Head
{
public:

	Head() {};
	virtual ~Head() {};

	virtual void Init() {};

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
	//アフィン変換
	RVector3 pos, rot, scale;

	//有効化フラグ
	bool isactive = false;

	//待機時間
	int waitTime = 0;

	//怒っているかどうか
	bool isAngree = false;

	//怒っている時間
	int AngreeTime = 0;

	//反撃するかどうか
	bool isCounter = false;

	//プレイヤーポインタ
	std::weak_ptr<ProtoPlayer> ptr;

	//イージング用変数
	//前に進む時
	float FrontEaseT = 0.0f;		//時間
	const float Length = 10.0f;		//変化量
	//ビンタされたとき
	float SlapEaseT = 0.0f;			//時間
	const float SlapLength = 1.0f;	//変化量
};

