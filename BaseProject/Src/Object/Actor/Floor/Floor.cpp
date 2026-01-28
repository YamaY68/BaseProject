#include "Floor.h"
#include"../Collider/ColliderBox.h"

#include<memory>

Floor::Floor()
	:
	size_(VGet(500.0f, 10.0f, 500.0f))
{
}

Floor::Floor(VECTOR size)
	:
	size_(size)
{
}

void Floor::SubInit(void)
{
	trans_.pos = VGet(0.0f, -10.0f, 0.0f);
}

void Floor::SubUpdate(void)
{
}

void Floor::SubDraw(void)
{
}

void Floor::SubRelease(void)
{
}

void Floor::InitCollider(void)
{
	ColliderInfo info =
	{
		SHAPE::BOX,
		&trans_,
		TAG::STAGE,
		Layer::STAGE,
		ColliderBase::SetMask({Layer::ACTOR,Layer::STAGE}),
		VGet(0.0f,0.0f,0.0f),
		VGet(0.0f,0.0f,0.0f),
		false,
		true
	};
	std::shared_ptr<ColliderBox>collider =
		std::make_shared<ColliderBox>(info, size_,this);
	ownColliders_.emplace(static_cast<int>(SHAPE::BOX), collider);

}
