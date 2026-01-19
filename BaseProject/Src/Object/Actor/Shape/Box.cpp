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

Box::Box(int color)
	:
	ShapeBase(color)
{
}

Box::Box(const VECTOR& halfSize)
	:
	ShapeBase(),
	halfSize_(halfSize)
{
}

Box::Box(const VECTOR& halfSize, int color)
	:
	ShapeBase(color),
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
    TAG::NONE,
    Layer::ACTOR,
    0,
    VGet(0.0f, 0.0f, 0.0f),
    VGet(0.0f, 0.0f, 0.0f),
    true,
    false,
    };
    std::shared_ptr<ColliderBox> collider =
		std::make_shared<ColliderBox>(info, halfSize_, this->color);
	ownColliders_.emplace(static_cast<int>(SHAPE::BOX), collider);
}
