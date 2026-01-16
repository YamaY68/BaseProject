#include "ColliderSphere.h"

ColliderSphere::ColliderSphere(void)
{
	// ƒRƒ‰ƒCƒ_î•ñ‰Šú‰»
	ColliderBase();
	radius_ = 0.0f;
}

ColliderSphere::ColliderSphere(ColliderInfo& info, float radius)
	:
	ColliderBase(info),
	radius_(radius)
{
}

ColliderSphere::ColliderSphere(ColliderInfo& info, float radius, int color)
	:
	ColliderBase(info, color),
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
