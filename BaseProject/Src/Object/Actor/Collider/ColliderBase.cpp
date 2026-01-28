#include "ColliderBase.h"
#include"../../Common/Transform.h"


ColliderBase::ColliderBase(ColliderInfo& info, ActorBase* actor)
	:
	colliderInfo_(info),
	actor_(actor)
{
}

void ColliderBase::Init(void)
{
}

void ColliderBase::Update(void)
{
}

void ColliderBase::Draw(void)
{
	// デバッグ描画
	DrawDebug(debugColor_);
}

void ColliderBase::Release(void)
{
}


VECTOR ColliderBase::GetRotPos(const VECTOR& localPos) const
{
	// 追従相手の回転に合わせて指定ローカル座標を回転し、
	// 基準座標に加えることでワールド座標へ変換
	VECTOR localRotPos = colliderInfo_.targetTransform->quaRot.PosAxis(localPos);
	return VAdd(colliderInfo_.targetTransform->pos, localRotPos);

}