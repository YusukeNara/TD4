#include "ColisionManager.h"
#include "BaseColider.h"
#include "Colider.h"
#include "ColliderShapeType.h"

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itrA, itrB;

	itrA = colliders.begin();
	for (; itrA != colliders.end(); ++itrA) {
		itrB = itrA;
		++itrB;
		for (; itrB != colliders.end(); ++itrB) {
			BaseCollider* colA = *itrA;
			BaseCollider* colB = *itrB;
			//”»’è•ªŠòŠÖ”
			CheckCollision(colA, colB);
		}
	}
}

void CollisionManager::CheckCollision(BaseCollider* colA, BaseCollider* colB)
{
	if (colA->GetColliderType() == COLIDER_TYPE_SPHERE) {
		CheckCollisionSphereTo(colA, colB);
	}
	else if (colA->GetColliderType() == COLIDER_TYPE_AABB) {
		CheckCollisionAABBTo(colA, colB);
	}
	else if (colA->GetColliderType() == COLIDER_TYPE_PLANE) {
		CheckCollisionPlaneTo(colA, colB);
	}
	else if (colA->GetColliderType() == COLIDER_TYPE_POLYGON) {
		CheckCollisionPolygonTo(colA, colB);
	}
	else if (colA->GetColliderType() == COLIDER_TYPE_RAY) {
		CheckCollisionRayTo(colA, colB);
	}


}

void CollisionManager::CheckCollisionSphereTo(BaseCollider* sphere, BaseCollider* colB)
{

	SphereCollider* a;
	SphereCollider* b;
	RVector3 inter;

	switch (colB->GetColliderType())
	{
	case COLIDER_TYPE_AABB:

		break;

	case COLIDER_TYPE_PLANE:
		break;

	case COLIDER_TYPE_POLYGON:
		break;

	case COLIDER_TYPE_RAY:
		break;

	case COLIDER_TYPE_SPHERE:
		//ƒLƒƒƒXƒg‚µ‚Ä”»’è‚ðŽæ‚é
		a = dynamic_cast<SphereCollider*>(sphere);
		b = dynamic_cast<SphereCollider*>(colB);

		if (RV3Colider::Colision2Sphere(a->sphere, b->sphere)) {



			

		}


		break;
	default:
		break;
	}


}

void CollisionManager::CheckCollisionPolygonTo(BaseCollider* polygon, BaseCollider* colB)
{
	switch (colB->GetColliderType())
	{
	case COLIDER_TYPE_AABB:
		break;

	case COLIDER_TYPE_PLANE:
		break;

	case COLIDER_TYPE_POLYGON:
		break;

	case COLIDER_TYPE_RAY:
		break;

	case COLIDER_TYPE_SPHERE:
		break;
	default:
		break;
	}
}

void CollisionManager::CheckCollisionAABBTo(BaseCollider* aabb, BaseCollider* colB)
{
	switch (colB->GetColliderType())
	{
	case COLIDER_TYPE_AABB:
		break;

	case COLIDER_TYPE_PLANE:
		break;

	case COLIDER_TYPE_POLYGON:
		break;

	case COLIDER_TYPE_RAY:
		break;

	case COLIDER_TYPE_SPHERE:
		break;
	default:
		break;
	}
}

void CollisionManager::CheckCollisionPlaneTo(BaseCollider* plane, BaseCollider* colB)
{
	switch (colB->GetColliderType())
	{
	case COLIDER_TYPE_AABB:
		break;

	case COLIDER_TYPE_PLANE:
		break;

	case COLIDER_TYPE_POLYGON:
		break;

	case COLIDER_TYPE_RAY:
		break;

	case COLIDER_TYPE_SPHERE:
		break;
	default:
		break;
	}
}

void CollisionManager::CheckCollisionRayTo(BaseCollider* ray, BaseCollider* colB)
{
	switch (colB->GetColliderType())
	{
	case COLIDER_TYPE_AABB:
		break;

	case COLIDER_TYPE_PLANE:
		break;

	case COLIDER_TYPE_POLYGON:
		break;

	case COLIDER_TYPE_RAY:
		break;

	case COLIDER_TYPE_SPHERE:
		break;
	default:
		break;
	}
}
