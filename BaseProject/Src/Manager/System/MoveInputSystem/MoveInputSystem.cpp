#include "MoveInputSystem.h"

#include"../../../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"
#include"../../../Object/Actor/Component/RigidBodyComponent/RigidBody.h"

#include"../../../Object/Actor/ActorBase.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Object/Actor/Camera/Camera.h"

MoveInputSystem::MoveInputSystem()
{
}

MoveInputSystem::~MoveInputSystem()
{
}
void MoveInputSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
    for (auto obj : objects)
    {
        if (obj->HasComponent<PlayerInputComponent>())
        {
            auto& input = obj->GetComponent<PlayerInputComponent>();
            auto& rb = obj->GetComponent<RigidBody>();

            VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

            // 入力方向の算出
            if (CheckHitKey(input.frontKey_)) moveDir.z += 1.0f;
            if (CheckHitKey(input.backKey_))  moveDir.z -= 1.0f;
            if (CheckHitKey(input.leftKey_))  moveDir.x -= 1.0f;
            if (CheckHitKey(input.rightKey_)) moveDir.x += 1.0f;

            // カメラ方向への変換
            const VECTOR cameraAngle = SceneManager::GetInstance().GetCamera()->GetAngles();
            MATRIX camYaw = MGetRotY(cameraAngle.y);
            moveDir = VTransform(moveDir, camYaw);

            if (VSize(moveDir) > 0.0f)
            {
                moveDir = VNorm(moveDir);

                // --- Velocity による移動 ---
                // 現在の速度に、入力方向の「加速」を加えるイメージ
                VECTOR currentVel = rb.GetVelocity();

                float accelSpeed = 1.2f; // 加速の鋭さ（調整してください）

                currentVel.x += moveDir.x * accelSpeed;
                currentVel.z += moveDir.z * accelSpeed;

                // 最高速度（MoveSpeed）を超えないように制限をかける
                float horizontalSpeed = sqrt(currentVel.x * currentVel.x + currentVel.z * currentVel.z);
                if (horizontalSpeed > rb.GetMoveSpeed())
                {
                    float ratio = rb.GetMoveSpeed() / horizontalSpeed;
                    currentVel.x *= ratio;
                    currentVel.z *= ratio;
                }

                rb.SetVelocity(currentVel);
            }

            // ジャンプ処理
            if (CheckHitKey(input.jumpKey_) && rb.IsGrounded())
            {
                VECTOR v = rb.GetVelocity();
                v.y = rb.GetJumpPower();
                rb.SetVelocity(v);
            }
        }
    }
}