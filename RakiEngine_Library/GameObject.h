#pragma once

#include <string>
#include <memory>

#include "NY_Object3D.h"
#include "Colider.h"


class GameObject
{
public:

	GameObject();
	virtual ~GameObject();

	//アプリケーション使用純粋仮想関数
	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Finalize() {};
	//読み込み専用
	virtual void Load() {};

	/// 仮想関数
	//衝突時コールバック
	virtual void OnCollision(const ColliderInfo *info){}


protected:
	//すべてのオブジェクトに必要なもののみ
	//その他のオブジェクトは派生クラスで定義すること



public:
	//描画オブジェクト
	std::shared_ptr<Object3d>		object3d;

	//衝突判定
	std::shared_ptr<BaseCollider>	collider;

	//変換情報
	RVector3 pos;

private:




};