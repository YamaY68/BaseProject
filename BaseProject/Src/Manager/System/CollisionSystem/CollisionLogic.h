#pragma once
#include<cstdint>
#include<DxLib.h>
#include<memory>
#include"CollisionPairType.h"
#include"../../../Object/Actor/Collider/ColliderShape.h"
#include"CollisionResult.h"
class ColliderBase;

class CollisionLogic
{
public:
	CollisionLogic(void);
	~CollisionLogic(void);

	static CollisionPairType GetCollisionPairType(
		SHAPE shapeA, SHAPE shapeB);

	static CollisionResult DispatchCollision(
		CollisionPairType pairType,
		const std::shared_ptr<ColliderBase>& colliderA,
		const std::shared_ptr<ColliderBase>& colliderB
	);
private:
	//衝突処理用関数群
//球×球
	static CollisionResult SphereToSphere(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//球×カプセル
	static CollisionResult SphereToCapsule(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//球×ボックス
	static CollisionResult SphereToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//カプセル×カプセル
	static CollisionResult CapsuleToCapsule(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//カプセル×ボックス
	static CollisionResult CapsuleToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//ボックス×ボックス
	static CollisionResult BoxToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);


	//球×モデル
	static CollisionResult SphereToModel(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);


	//カプセル×モデル
	static CollisionResult CapsuleToModel(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);


	//ボックス×モデル
	static CollisionResult BoxToModel(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);


	//モデル×モデル
	static CollisionResult ModelToModel(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
};

