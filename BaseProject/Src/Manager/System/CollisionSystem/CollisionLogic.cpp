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

CollisionResult CollisionLogic::SphereToSphere(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b) {
    CollisionResult result;
    auto sphereA = std::dynamic_pointer_cast<ColliderSphere>(a);
    auto sphereB = std::dynamic_pointer_cast<ColliderSphere>(b);
    if (!sphereA || !sphereB) return result;

    VECTOR diff = VSub(sphereB->GetFollow()->pos, sphereA->GetFollow()->pos); // AからBへ
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float radiusSum = sphereA->GetRadius() + sphereB->GetRadius();

    if (distSq > radiusSum * radiusSum) return result;

    result.isHit = true;
    float dist = sqrtf(distSq);
    if (dist < 0.0001f) {
        result.normal = VGet(0, 1, 0); // 真上の場合は便宜上
        result.penetration = radiusSum;
    }
    else {
        result.normal = VScale(diff, 1.0f / dist); // AからBへ向かう法線
        result.penetration = radiusSum - dist;
    }
    return result;
}

CollisionResult CollisionLogic::SphereToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;
    auto sphere = std::dynamic_pointer_cast<ColliderSphere>(a);
    auto capsule = std::dynamic_pointer_cast<ColliderCapsule>(b);
    if (!sphere || !capsule) return result;

    // A: 球の情報
    VECTOR sPos = sphere->GetFollow()->pos;
    float sRadius = sphere->GetRadius();

    // B: カプセルの情報
    VECTOR cA = capsule->GetPosDown();
    VECTOR cB = capsule->GetPosTop();
    float cRadius = capsule->GetRadius();

    // --- 線分 AB 上の最短地点 P を求める ---
    VECTOR segment = VSub(cB, cA);
    VECTOR toSphere = VSub(sPos, cA);
	float segLenSq = segment.x * segment.x + segment.y * segment.y + segment.z * segment.z;

    float t = (segLenSq > 0.0001f) ? VDot(toSphere, segment) / segLenSq : 0.0f;
    t = std::clamp(t, 0.0f, 1.0f);

    // カプセルの中心線上の最短地点 P
    VECTOR p = VAdd(cA, VScale(segment, t));

    // --- 距離判定 ---
    // ★重要：法線の向きを「球(A)からカプセル上の点(B)へ」にする
    VECTOR diff = VSub(p, sPos);
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float radiusSum = sRadius + cRadius;

    if (distSq > radiusSum * radiusSum) return result;

    result.isHit = true;
    float dist = sqrtf(distSq);

    if (dist < 0.0001f) {
        // 球の中心がカプセルの線分と完全に重なった場合
        // 真上に弾き飛ばしてガクガクを防止する
        result.normal = VGet(0, 1, 0);
        result.penetration = radiusSum;
    }
    else {
        // 球(A)からカプセル(B)へ向かう法線
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = radiusSum - dist;
    }

    return result;
}
CollisionResult CollisionLogic::SphereToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b) {
    CollisionResult result;
    auto sphere = std::dynamic_pointer_cast<ColliderSphere>(a);
    auto box = std::dynamic_pointer_cast<ColliderBox>(b);
    if (!sphere || !box) return result;

    VECTOR sPos = sphere->GetFollow()->pos;
    VECTOR bPos = box->GetRotPos(box->GetColliderInfo().localPos);
    VECTOR half = box->GetHalfSize();

    // Box上の最短地点をクランプで求める
    VECTOR closest;
    closest.x = (std::max)(bPos.x - half.x, (std::min)(sPos.x, bPos.x + half.x));
    closest.y = (std::max)(bPos.y - half.y, (std::min)(sPos.y, bPos.y + half.y));
    closest.z = (std::max)(bPos.z - half.z, (std::min)(sPos.z, bPos.z + half.z));

    VECTOR diff = VSub(closest, sPos); // 球(A)からBox(B)の最短地点へ
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float sRadius = sphere->GetRadius();

    if (distSq > sRadius * sRadius) return result;

    result.isHit = true;
    float dist = sqrtf(distSq);
    // 法線は「球(A)からBox(B)へ」
    if (dist < 0.0001f) {
        // 球の中心が箱の中にある場合、球から中心への逆ベクトル
        result.normal = VScale(VSub(sPos, bPos), -1.0f);
        result.penetration = sRadius;
    }
    else {
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = sRadius - dist;
    }
    return result;
}

CollisionResult CollisionLogic::CapsuleToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;
    auto capA = std::dynamic_pointer_cast<ColliderCapsule>(a);
    auto capB = std::dynamic_pointer_cast<ColliderCapsule>(b);
    if (!capA || !capB) return result;

    // カプセルAの線分 (p1 - q1) と半径
    VECTOR p1 = capA->GetPosDown();
    VECTOR q1 = capA->GetPosTop();
    float r1 = capA->GetRadius();

    // カプセルBの線分 (p2 - q2) と半径
    VECTOR p2 = capB->GetPosDown();
    VECTOR q2 = capB->GetPosTop();
    float r2 = capB->GetRadius();

    // --- 2線分間の最短距離地点を算出 (数学的な最短距離アルゴリズム) ---
    VECTOR d1 = VSub(q1, p1); // 線分Aの方向ベクトル
    VECTOR d2 = VSub(q2, p2); // 線分Bの方向ベクトル
    VECTOR r = VSub(p1, p2);

    float a11 = VDot(d1, d1); // 線分Aの長さの2乗
    float e = VDot(d2, d2);   // 線分Bの長さの2乗
    float f = VDot(d2, r);

    float s = 0.0f;
    float t = 0.0f;
    const float EPSILON = 0.0001f;

    // 線分A, Bそれぞれの上の位置パラメータ s, t (0.0 ～ 1.0) を求める
    if (a11 <= EPSILON && e <= EPSILON) {
        s = t = 0.0f;
    }
    else if (a11 <= EPSILON) {
        s = 0.0f;
        t = std::clamp(f / e, 0.0f, 1.0f);
    }
    else {
        float c = VDot(d1, r);
        if (e <= EPSILON) {
            t = 0.0f;
            s = std::clamp(-c / a11, 0.0f, 1.0f);
        }
        else {
            float b12 = VDot(d1, d2);
            float denom = a11 * e - b12 * b12;

            // 平行でない場合
            if (denom != 0.0f) {
                s = std::clamp((b12 * f - c * e) / denom, 0.0f, 1.0f);
            }
            else {
                s = 0.0f;
            }

            t = (b12 * s + f) / e;

            // tが範囲外なら再クランプしてsを調整
            if (t < 0.0f) {
                t = 0.0f;
                s = std::clamp(-c / a11, 0.0f, 1.0f);
            }
            else if (t > 1.0f) {
                t = 1.0f;
                s = std::clamp((b12 - c) / a11, 0.0f, 1.0f);
            }
        }
    }

    // 最短地点の座標を確定
    VECTOR closestA = VAdd(p1, VScale(d1, s));
    VECTOR closestB = VAdd(p2, VScale(d2, t));

    // --- 最短地点同士の距離で球判定 ---
    VECTOR diff = VSub(closestB, closestA); // AからBへ向かうベクトル
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float radiusSum = r1 + r2;

    if (distSq > radiusSum * radiusSum) return result; // 当たっていない

    result.isHit = true;
    float dist = sqrtf(distSq);

    // 衝突法線と貫通深度の設定
    if (dist < EPSILON) {
        // 完全に重なっている場合は、とりあえず上方向
        result.normal = VGet(0.0f, 1.0f, 0.0f);
        result.penetration = radiusSum;
    }
    else {
        // 法線は「AからBへ押し出す向き」に統一
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = radiusSum - dist;
    }

    return result;
}

CollisionResult CollisionLogic::CapsuleToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;
    auto capsule = std::dynamic_pointer_cast<ColliderCapsule>(a);
    auto box = std::dynamic_pointer_cast<ColliderBox>(b);
    if (!capsule || !box) return result;

    // 1. 各情報の取得
    VECTOR p = capsule->GetPosDown(); // 線分始点
    VECTOR q = capsule->GetPosTop();  // 線分終点
    float cRadius = capsule->GetRadius();

    VECTOR bPos = box->GetRotPos(box->GetColliderInfo().localPos);
    VECTOR halfSize = box->GetHalfSize();

    // 2. 線分 pq 上で、ボックス中心に最も近い点を探すためのパラメータ t を計算
    VECTOR d = VSub(q, p);
	float dLenSq = d.x * d.x + d.y * d.y + d.z * d.z;
    float t = 0.0f;

    if (dLenSq > 0.0001f) {
        // ボックスの中心に向かうベクトルとの投影から、最適な t を算出
        // ※厳密には各軸ごとのクランプが必要ですが、実用的には
        // ボックス中心への投影地点を起点にするのが最も安定します
        t = VDot(VSub(bPos, p), d) / dLenSq;
        t = std::clamp(t, 0.0f, 1.0f);
    }

    // カプセル線分上の最短候補点 P
    VECTOR closestOnSegment = VAdd(p, VScale(d, t));

    // 3. 点 P からボックス上の最短地点 Q を求める (SphereToBoxと同じロジック)
    VECTOR closestOnBox;
    closestOnBox.x = std::clamp(closestOnSegment.x, bPos.x - halfSize.x, bPos.x + halfSize.x);
    closestOnBox.y = std::clamp(closestOnSegment.y, bPos.y - halfSize.y, bPos.y + halfSize.y);
    closestOnBox.z = std::clamp(closestOnSegment.z, bPos.z - halfSize.z, bPos.z + halfSize.z);

    // 4. P と Q の距離で判定
    VECTOR diff = VSub(closestOnBox, closestOnSegment); // A(カプセル)からB(ボックス)へ
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    if (distSq > cRadius * cRadius) return result;

    result.isHit = true;
    float dist = sqrtf(distSq);

    if (dist < 0.0001f) {
        // 埋まり込んでいる場合、カプセルの中心から箱の法線方向へ押し出す
        result.normal = VGet(0, 1, 0);
        result.penetration = cRadius;
    }
    else {
        result.normal = VScale(diff, 1.0f / dist); // AからBへの法線
        result.penetration = cRadius - dist;
    }

    return result;
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
