#include "PhysicsSystem.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Object/Actor/Component/RigidBodyComponent/RigidBody.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
    const float dt = 1.0f / 60.0f;
    const VECTOR GRAVITY = VGet(0.0f, -9.8f, 0.0f); 

    for (auto& obj : objects)
    {
		// RigidBodyコンポーネントを持たないオブジェクトはスキップ
        if (!obj->HasComponent<RigidBody>()) continue;
        auto& rigidbody = obj->GetComponent<RigidBody>();

		// 前フレームの位置を保存
        obj->GetTransform().prevPos = obj->GetTransform().pos;

        //ダイナミックな場合のみ重力を適応
        if (rigidbody.GetBodyType() == RigidBody::BodyType::DYNAMIC)
        {
            //Velocityの更新（重力による加速度の加算）
            if (rigidbody.IsUseGravity() && !rigidbody.IsGrounded())
            {
                rigidbody.SetVelocity(VAdd(rigidbody.GetVelocity(), VScale(GRAVITY, dt)));
            }

            //摩擦の適用
            VECTOR velocity = rigidbody.GetVelocity();
            velocity.x *= 0.98f;
            velocity.z *= 0.98f;
            rigidbody.SetVelocity(velocity);
        }

        // --- 移動量の集計 ---

        //持続的な速度 (Velocity)
        VECTOR velMove = rigidbody.GetVelocity();

        //そのフレーム限りの力 (Force) から速度への変換
        VECTOR forceVel = rigidbody.GetForce();

        //合計の「秒間速度」
        VECTOR totalVelocity = VAdd(velMove, forceVel);

        // --- 最終的な座標更新 ---
        // (合計秒間速度) * デルタタイム = このフレームの移動距離
        VECTOR frameMovement = VScale(totalVelocity, dt);
        obj->GetTransform().pos = VAdd(obj->GetTransform().pos, frameMovement);

        rigidbody.ClearForce();
        rigidbody.SetGrounded(false);
    }
}

void PhysicsSystem::Resolve(std::vector<CollisionSolver>& solver)
{
    for (auto& s : solver)
    {
        auto& actorA = s.actorA;
        auto& actorB = s.actorB;
        auto& rbA = actorA->GetComponent<RigidBody>();
        auto& rbB = actorB->GetComponent<RigidBody>();
        auto& res = s.result;

        float weightA = (rbA.GetBodyType() == RigidBody::BodyType::DYNAMIC) ? rbA.GetInverseMass() : 0.0f;
        float weightB = (rbB.GetBodyType() == RigidBody::BodyType::DYNAMIC) ? rbB.GetInverseMass() : 0.0f;

        float totalWeight = weightA + weightB;
        if (totalWeight <= 0.0f) continue;

        float pushA = weightA / totalWeight;
        float pushB = weightB / totalWeight;

        // --- Aの更新 ---
        if (pushA > 0.0f)
        {
            actorA->GetTransform().pos = VSub(actorA->GetTransform().pos, VScale(res.normal, res.penetration * pushA));

            // 接地判定：法線が上向き（0.5f以上）なら「床」とみなす
            // normalはAからBへの向きなので、Aから見て床(B)が下にある場合、normal.yは負の値
            // その逆方向(-normal.y)が正ならAは上に押し戻されている
            if (res.normal.y < -0.5f)
            {
                rbA.SetGrounded(true);
                // 落下速度を止める
                VECTOR vel = rbA.GetVelocity();
                if (vel.y < 0.0f) vel.y = 0.0f;
                rbA.SetVelocity(vel);
            }
        }

        // --- Bの更新 ---
        if (pushB > 0.0f)
        {
            actorB->GetTransform().pos = VAdd(actorB->GetTransform().pos, VScale(res.normal, res.penetration * pushB));

            // Bから見てAが上にいる（Bが下から支えている）場合
            if (res.normal.y > 0.5f)
            {
                rbB.SetGrounded(true);
                VECTOR vel = rbB.GetVelocity();
                if (vel.y < 0.0f) vel.y = 0.0f;
                rbB.SetVelocity(vel);
            }
        }
    }
    solver.clear();
}