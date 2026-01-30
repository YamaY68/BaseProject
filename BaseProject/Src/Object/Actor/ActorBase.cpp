#include "ActorBase.h"

ActorBase::ActorBase(void)
{
}

ActorBase::~ActorBase(void)
{
}

void ActorBase::Load(void)
{
}

void ActorBase::Init(void)
{
	SubInit();
	InitCollider();
}

void ActorBase::Update(void)
{
	SubUpdate();
	trans_.Update();
}

void ActorBase::Draw(void)
{
	SubDraw();
}

void ActorBase::Release(void)
{
	SubRelease();
}

void ActorBase::SetOwnerActor2Colliders(void)
{
}
