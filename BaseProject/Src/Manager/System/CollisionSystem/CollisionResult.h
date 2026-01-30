#pragma once
#include<DxLib.h>
struct CollisionResult
{
	bool isHit = false;
	
	VECTOR normal = VGet(0.0f, 0.0f, 0.0f);

	float penetration = 0.0f;
};