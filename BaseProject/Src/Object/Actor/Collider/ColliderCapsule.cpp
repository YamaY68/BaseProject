#include "ColliderCapsule.h"
#include<DxLib.h>
ColliderCapsule::ColliderCapsule(void)
{
	// コライダ情報初期化
	ColliderBase();
	radius_ = 0.0f;
	localPosTop_ = VGet(0.0f, 0.0f, 0.0f);
	localPosDown_ = VGet(0.0f, 0.0f, 0.0f);
}

ColliderCapsule::ColliderCapsule(ColliderInfo& info, float radius, const VECTOR& localPosTop, const VECTOR& localPosDown)
    :
	ColliderBase(info),
	radius_(radius),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown)
{
}

ColliderCapsule::ColliderCapsule(ColliderInfo& info, float radius, const VECTOR& localPosTop, const VECTOR& localPosDown, int color)
	:
	ColliderBase(info, color),
	radius_(radius),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown)
{
}

ColliderCapsule::~ColliderCapsule(void)
{
}

float ColliderCapsule::GetHeight(void) const
{
	return VSize(VSub(localPosTop_, localPosDown_));
}

VECTOR ColliderCapsule::GetPosTop(void) const
{
	return GetRotPos(localPosTop_);
}

VECTOR ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

VECTOR ColliderCapsule::GetCenter(void) const
{
    VECTOR top = GetPosTop();
    VECTOR down = GetPosDown();
    VECTOR diff = VSub(top, down);
    return VAdd(down, VScale(diff, 0.5f));
}

void ColliderCapsule::DrawDebug(int color)
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	// 球
	DrawSphere3D(top, radius_, 8, color, color, false);
	DrawSphere3D(down, radius_, 8, color, color, false);

	// 軸
	VECTOR axis = VNorm(VSub(top, down));

	// 安全な up
	VECTOR up = VGet(0, 1, 0);
	if (fabs(VDot(axis, up)) > 0.99f)
	{
		up = VGet(1, 0, 0);
	}

	// 側面方向
	VECTOR right = VNorm(VCross(up, axis));
	VECTOR forward = VCross(axis, right);

	// 4本の側面線
	auto DrawSide = [&](const VECTOR& dir)
		{
			VECTOR s = VAdd(top, VScale(dir, radius_));
			VECTOR e = VAdd(down, VScale(dir, radius_));
			DrawLine3D(s, e, color);
		};

	DrawSide(right);
	DrawSide(VScale(right, -1));
	DrawSide(forward);
	DrawSide(VScale(forward, -1));

	// 中心
	DrawSphere3D(GetCenter(), 4.0f, 6, color, color, false);
}
