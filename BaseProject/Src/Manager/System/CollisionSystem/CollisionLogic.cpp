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
    // ìØÇ∂å`èÛìØém  
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

    // èáèòÇãzé˚Åiè¨Ç≥Ç¢ÇŸÇ§ÇAÇ…Åj  
    if (shapeA > shapeB)
        std::swap(shapeA, shapeB);

    // àŸÇ»ÇÈå`èÛìØém  
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
    return CollisionResult();
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
