#include "CollisionLogic.h"
#include <type_traits>  
#include <algorithm>  

#include"../../../Object/Actor/Collider/ColliderInfo.h"
#include"../../../Object/Actor/Collider/ColliderBase.h"

#include"../../../Object/Actor/Collider/ColliderSphere.h"
#include"../../../Object/Actor/Collider/ColliderBox.h"
#include"../../../Object/Actor/Collider/ColliderCapsule.h"
CollisionLogic::CollisionLogic(void)
{
}

CollisionLogic::~CollisionLogic(void)
{
}

CollisionPairType CollisionLogic::GetCollisionPairType(SHAPE shapeA, SHAPE shapeB)
{
    // 同じ形状同士  
    if (shapeA == shapeB)
    {
        switch (shapeA)
        {
        case SHAPE::SPHERE:   return CollisionPairType::SPHERE_SPHERE;
        case SHAPE::CAPSULE: return CollisionPairType::CAPSULE_CAPSULE;
        case SHAPE::BOX:     return CollisionPairType::BOX_BOX;
        default:             return CollisionPairType::NONE;
        }
    }

    // 順序を吸収（小さいほうをAに）  
    if (shapeA > shapeB)
        std::swap(shapeA, shapeB);

    // 異なる形状同士  
    if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::CAPSULE)
        return CollisionPairType::SPHERE_CAPSULE;

    if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::BOX)
        return CollisionPairType::SPHERE_BOX;

    if (shapeA == SHAPE::CAPSULE && shapeB == SHAPE::BOX)
        return CollisionPairType::CAPSULE_BOX;

    if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::MODEL)
        return CollisionPairType::SPHERE_MODEL;

    if (shapeA == SHAPE::CAPSULE && shapeB == SHAPE::MODEL)
        return CollisionPairType::CAPSULE_MODEL;

    if (shapeA == SHAPE::BOX && shapeB == SHAPE::MODEL)
        return CollisionPairType::BOX_MODEL;

    if (shapeA == SHAPE::MODEL && shapeB == SHAPE::MODEL)
        return CollisionPairType::MODEL_MODEL;

    return CollisionPairType::NONE;
}

CollisionResult CollisionLogic::DispatchCollision(CollisionPairType pairType, 
    const std::shared_ptr<ColliderBase>& a,
    const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;
    switch (pairType)
    {
    case CollisionPairType::NONE:
        break;
    case CollisionPairType::SPHERE_SPHERE:
        result = SphereToSphere(a, b);
        break;
    case CollisionPairType::SPHERE_CAPSULE:
        result = SphereToCapsule(a, b);
        break;
    case CollisionPairType::SPHERE_BOX:
        result = SphereToBox(a, b);
        break;
    case CollisionPairType::CAPSULE_CAPSULE:
        result = CapsuleToCapsule(a, b);
        break;
    case CollisionPairType::CAPSULE_BOX:
        result = CapsuleToBox(a, b);
        break;
    case CollisionPairType::BOX_BOX:
        result = BoxToBox(a, b);
        break;
    case CollisionPairType::SPHERE_MODEL:
        result = SphereToModel(a, b);
        break;
    case CollisionPairType::CAPSULE_MODEL:
        result = CapsuleToModel(a, b);
        break;
    case CollisionPairType::BOX_MODEL:
        result = BoxToModel(a, b);
        break;
    case CollisionPairType::MODEL_MODEL:
        result = ModelToModel(a, b);
        break;
    default:
        break;
    }

    return result;
}

CollisionResult CollisionLogic::SphereToSphere(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
	CollisionResult result;

	auto sphereA = std::dynamic_pointer_cast<ColliderSphere>(a);
	auto sphereB = std::dynamic_pointer_cast<ColliderSphere>(b);

	if (!sphereA || !sphereB)
	{
		return result;
	}

	VECTOR posA = sphereA->GetFollow()->pos;
	VECTOR posB = sphereB->GetFollow()->pos;

	float radiusA = sphereA->GetRadius();
	float radiusB = sphereB->GetRadius();

	VECTOR diff = VSub(posB, posA);

	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	float radiusSum = radiusA + radiusB;

    if (distSq > radiusSum * radiusSum)return result;
	result.isHit = true;

    if (distSq < 0.01f)
    {
		result.normal = VGet(1.0f, 0.0f, 0.0f);
		result.penetration = radiusSum;
		return result;
    }
    else
    {
		float dist = sqrtf(distSq);
		result.normal = VScale(diff, 1.0f / dist);
		result.penetration = radiusSum - dist;
    }
    return result;
}

CollisionResult CollisionLogic::SphereToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::SphereToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::CapsuleToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::CapsuleToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::BoxToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;
    auto boxA = std::dynamic_pointer_cast<ColliderBox>(a);
    auto boxB = std::dynamic_pointer_cast<ColliderBox>(b);
    if (!boxA || !boxB) return result;

    // 1. 各ボックスの中心座標（ワールド）を取得
    VECTOR posA = boxA->GetRotPos(boxA->GetColliderInfo().localPos);
    VECTOR posB = boxB->GetRotPos(boxB->GetColliderInfo().localPos);

    // 2. 半辺長（Half Size）を取得
    VECTOR hA = boxA->GetHalfSize();
    VECTOR hB = boxB->GetHalfSize();

    // 3. 各軸（X, Y, Z）で重なりをチェック
    // 中心間の距離が、お互いの半径（半辺長）の合計より小さければ重なっている
    float dx = fabsf(posA.x - posB.x);
    float dy = fabsf(posA.y - posB.y);
    float dz = fabsf(posA.z - posB.z);

    float overlapX = (hA.x + hB.x) - dx;
    float overlapY = (hA.y + hB.y) - dy;
    float overlapZ = (hA.z + hB.z) - dz;

    // いずれかの軸で隙間があれば当たっていない
    if (overlapX < 0 || overlapY < 0 || overlapZ < 0) return result;

    // 全ての軸で重なっていれば衝突
    result.isHit = true;

    // 4. 押し出し情報（penetration & normal）の計算
    // 最も重なりが小さい軸を「衝突法線」とするのが物理演算の定石
    if (overlapX < overlapY && overlapX < overlapZ) {
        result.penetration = overlapX;
        result.normal = VGet((posB.x > posA.x) ? 1.0f : -1.0f, 0.0f, 0.0f);
    }
    else if (overlapY < overlapZ) {
        result.penetration = overlapY;
        result.normal = VGet(0.0f, (posB.y > posA.y) ? 1.0f : -1.0f, 0.0f);
    }
    else {
        result.penetration = overlapZ;
        result.normal = VGet(0.0f, 0.0f, (posB.z > posA.z) ? 1.0f : -1.0f);
    }

    return result;
}

CollisionResult CollisionLogic::SphereToModel(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::CapsuleToModel(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::BoxToModel(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionResult CollisionLogic::ModelToModel(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}
