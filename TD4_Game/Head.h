#pragma once
#include <RVector.h>
#include <memory>

#include "ProtoPlayer.h"

class ProtoPlayer;

class Head
{
public:

	Head() {};
	virtual ~Head() {};

	virtual void Init() {};

	virtual void Update() {};

	virtual void Draw() {};

	virtual void Finalize() {};

	void SetPlayer(ProtoPlayer *ptr) {
		this->ptr = ptr;
	}

	void Activate() {
		isactive = true;
	}

	virtual void HairCut() {};

	virtual void Slap() {};

public:
	//アフィン変換
	RVector3 pos, rot, scale;

	//有効化フラグ
	bool isactive = false;
	bool isSlaped = false;

	//プレイヤーポインタ
	ProtoPlayer *ptr;

};

