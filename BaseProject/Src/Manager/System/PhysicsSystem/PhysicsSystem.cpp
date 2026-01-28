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
    const VECTOR GRAVITY = VGet(0.0f, -0.98f, 0.0f);

    for (auto& obj : objects)
    {
        if (!obj->HasComponent<RigidBody>()) continue;
        auto& rigidbody = obj->GetComponent<RigidBody>();

        // 1. 重力の計算（オプション）
        if (rigidbody.IsUseGravity() && !rigidbody.IsGrounded())
        {
            // 重力も一つの「力」として蓄積
            rigidbody.AddForce(VScale(GRAVITY, rigidbody.GetMass()));
        }

        // 2. 蓄積された力から「このフレームの移動量」を算出
        // 加速度 a = F / m
        VECTOR acceleration = VScale(rigidbody.GetForce(), rigidbody.GetInverseMass());

        // 移動量 d = a * dt * dt (または簡易的に a * dt)
        // ※Velocityを持たない場合、加速度が直接移動の勢いになります
        VECTOR frameMovement = VScale(acceleration, 1);

        // 3. 座標の更新
        VECTOR currentPos = obj->GetTransform().pos;
        obj->GetTransform().pos = VAdd(currentPos, frameMovement);

        // 4. 重要：力の初期化
        // これをしないと、次のフレームも同じ方向に加速し続けてしまいます
        rigidbody.ClearForce();
    }
}
