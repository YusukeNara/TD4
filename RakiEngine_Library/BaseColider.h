#pragma once

#include "RVector.h"
#include "ColliderShapeType.h"
#include "ColliderInfo.h"

class GameObject;

//コライダー基底クラス
class BaseCollider
{
public:
	BaseCollider()			= default;
	virtual ~BaseCollider()	= default;

	inline void SetObject(GameObject* object) {
		this->object = object;
	}

	GameObject *GetObject3d() { 
		return object; 
	}

	virtual void Update() = 0;

	inline COLIDER_TYPE GetColliderType() { return type; }

	virtual void OnColision(const ColliderInfo& info);

protected:
	// コライダー登録オブジェクト
	GameObject* object;

	// コライダーの種類
	COLIDER_TYPE type = COLIDER_TYPE_UNKNOWN;





};