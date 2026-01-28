#include "Box.h"
#include<memory>
#include"../Collider/ColliderInfo.h"
#include"../Collider/ColliderBase.h"
#include"../Collider/ColliderBox.h"

Box::Box(void)
	:
	ShapeBase(),
	halfSize_({100.0f,100.0f,100.0f})
{
}



Box::Box(const VECTOR& halfSize)
	:
	ShapeBase(),
	halfSize_(halfSize)
{
}




Box::~Box(void)
{
}

void Box::SubInit(void)
{
}

void Box::SubUpdate(void)
{
}

void Box::SubDraw(void)
{
}

void Box::SubRelease(void)
{
}

void Box::InitCollider(void)
{
    ColliderInfo info = {
    SHAPE::BOX,
    &trans_,
    TAG::PLAYER,
    Layer::ACTOR,
    ColliderBase::SetMask({Layer::ACTOR,Layer::STAGE}),
    VGet(0.0f, 0.0f, 0.0f),
    VGet(0.0f, 0.0f, 0.0f),
    true,
    true,
    };
    std::shared_ptr<ColliderBox> collider =
		std::make_shared<ColliderBox>(info, halfSize_, this);
	ownColliders_.emplace(static_cast<int>(SHAPE::BOX), collider);
}
