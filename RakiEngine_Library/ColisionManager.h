#pragma once

#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:

	CollisionManager() = default;
	~CollisionManager() = default;

	//これを単体で呼び出せば、コライダーだけを追加することも可能
	inline void AddCollider(BaseCollider* col) {
		colliders.push_front(col);
	}

	//管理から外すのみ
	inline void RemoveColider(BaseCollider* col) {
		colliders.remove(col);
	}

	//すべての衝突判定を一括で行う
	void CheckAllCollisions();


private:

	std::forward_list<BaseCollider*> colliders;

	//判定分岐関数
	void CheckCollision(BaseCollider* colA, BaseCollider* colB);

	//各種判定用関数
	//dynamic_castして判定を取り、tureの場合OnCollisionを実行する

	void CheckCollisionSphereTo(BaseCollider* sphere, BaseCollider* colB);

	void CheckCollisionPolygonTo(BaseCollider* polygon, BaseCollider* colB);

	void CheckCollisionAABBTo(BaseCollider* aabb, BaseCollider* colB);

	void CheckCollisionPlaneTo(BaseCollider* plane, BaseCollider* colB);

	void CheckCollisionRayTo(BaseCollider* ray, BaseCollider* colB);

};

