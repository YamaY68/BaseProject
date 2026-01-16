#include "ColliderBase.h"
#include"../../Common/Transform.h"

ColliderBase::ColliderBase(void)
{
	// コライダ情報初期化
	colliderInfo_.shape = SHAPE::NONE;
	colliderInfo_.targetTransform = nullptr;
	colliderInfo_.tag = TAG::NONE;
	colliderInfo_.layer = Layer::NONE;
	colliderInfo_.mask = 0;
	colliderInfo_.localPos = VGet(0.0f, 0.0f, 0.0f);
	colliderInfo_.localRot = VGet(0.0f, 0.0f, 0.0f);
	colliderInfo_.isTrigger = false;
	colliderInfo_.isActive = false;

}

ColliderBase::ColliderBase(ColliderInfo& info)
	:
	colliderInfo_(info)
{
	debugColor_ = COLOR_INVALID;
}

ColliderBase::ColliderBase(ColliderInfo& info, int color)
	:
	colliderInfo_(info),
	debugColor_(color)
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