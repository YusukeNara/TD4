#include "BaseColider.h"
#include "GameObject.h"

void BaseCollider::OnColision(const ColliderInfo& info)
{
	object->OnCollision(&info);
}
