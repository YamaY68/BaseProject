#include "ColliderSphere.h"


ColliderSphere::ColliderSphere(ColliderInfo& info, float radius, ActorBase* actor)
	:
	ColliderBase(info, actor),
	radius_(radius)
{
}



ColliderSphere::~ColliderSphere(void)
{
}

void ColliderSphere::DrawDebug(int color)
{
	DrawSphere3D(
		GetRotPos(colliderInfo_.localPos),
		radius_,
		16,
		color,
		color,
		false
	);
}
