#include "Capsule.h"
#include<memory>
#include"../../../Object/Actor/Collider/ColliderCapsule.h"
Capsule::Capsule(void)
	:
	ShapeBase(),
	radius_(50.0f),
	localPosTop_(VGet(0.0f, 50.0f, 0.0f)),
	localPosDown_(VGet(0.0f, -50.0f, 0.0f))
{
}



Capsule::Capsule(float radius, VECTOR localPosTop, VECTOR localPosDown)
	:
	ShapeBase(),
	radius_(radius),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown)
{
}


Capsule::~Capsule(void)
{
}

void Capsule::SubInit(void)
{
}

void Capsule::SubUpdate(void)
{
}

void Capsule::SubDraw(void)
{
}

void Capsule::SubRelease(void)
{
}

void Capsule::InitCollider(void)
{

	ColliderInfo info = {
	SHAPE::CAPSULE,
	&trans_,
	TAG::NONE,
	Layer::NONE,
	0,
	VGet(0.0f, 0.0f, 0.0f),
	VGet(0.0f, 0.0f, 0.0f),
	true,
	false,
	};
	std::shared_ptr<ColliderCapsule> collider =
		std::make_shared<ColliderCapsule>(info, radius_,localPosTop_,localPosDown_,this);
	ownColliders_.emplace(static_cast<int>(SHAPE::CAPSULE), collider);

}
