#include "ColliderCapsule.h"
#include<DxLib.h>


ColliderCapsule::ColliderCapsule(ColliderInfo& info, float radius,  VECTOR& localPosTop,  VECTOR& localPosDown, ActorBase* actor)
	:
	ColliderBase(info, actor), radius_(radius), localPosTop_(localPosTop), localPosDown_(localPosDown)
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

	// ãÖ
	DrawSphere3D(top, radius_, 8, color, color, false);
	DrawSphere3D(down, radius_, 8, color, color, false);

	// é≤
	VECTOR axis = VNorm(VSub(top, down));

	// à¿ëSÇ» up
	VECTOR up = VGet(0, 1, 0);
	if (fabs(VDot(axis, up)) > 0.99f)
	{
		up = VGet(1, 0, 0);
	}

	// ë§ñ ï˚å¸
	VECTOR right = VNorm(VCross(up, axis));
	VECTOR forward = VCross(axis, right);

	// 4ñ{ÇÃë§ñ ê¸
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

	// íÜêS
	DrawSphere3D(GetCenter(), 4.0f, 6, color, color, false);
}
