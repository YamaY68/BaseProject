#pragma once
#include "ColliderInfo.h"
#include "ColliderShape.h"  
#include "ColliderLayer.h"  

#include "../ActorTag.h"  
#include "../../Common/Transform.h"


struct ColliderInfo
{
	SHAPE shape;	//形状
	Transform* targetTransform;	//ターゲットのTransform
	TAG tag;	//当たり判定のタグ
	Layer layer;	//当たり判定を行うレイヤーのマスク
	uint32_t mask;	//当たり判定を受け付けるレイヤーのマスク
	VECTOR localPos;	//ローカル座標
	VECTOR localRot;	//ローカル回転
	bool isTrigger;	//トリガー判定かどうか
	bool isActive;	//有効かどうか
	int debugColor = GetColor(255, 0, 0);
};