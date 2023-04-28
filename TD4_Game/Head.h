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

	//プレイヤーポインタ
	std::weak_ptr<ProtoPlayer> ptr;

};

