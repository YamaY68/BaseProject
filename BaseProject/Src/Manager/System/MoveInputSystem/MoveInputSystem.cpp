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
		//プレイヤー入力コンポーネントを持っているか
		if (obj->HasComponent<PlayerInputComponent>())
		{
			auto& input = obj->GetComponent<PlayerInputComponent>();
			auto& rb = obj->GetComponent<RigidBody>();

			VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

			//入力に応じて移動方向を設定
			if (CheckHitKey(input.frontKey_))moveDir.z += 1.0f;
			if (CheckHitKey(input.backKey_))moveDir.z -= 1.0f;
			if (CheckHitKey(input.leftKey_))moveDir.x -= 1.0f;
			if (CheckHitKey(input.rightKey_))moveDir.x += 1.0f;
			if (CheckHitKey(input.upKey_))moveDir.y += 1.0f;
			if (CheckHitKey(input.downKey_))moveDir.y -= 1.0f;
			
			const VECTOR cameraAngle = SceneManager::GetInstance().GetCamera()->GetAngles();
			MATRIX camYaw = MGetRotY(cameraAngle.y);
			moveDir = VTransform(moveDir, camYaw);

			if (VSize(moveDir) > 0.0f)
			{
				moveDir = VNorm(moveDir);
			}

			if (CheckHitKey(input.jumpKey_) && rb.IsGrounded())
			{
				rb.SetVelocity(VAdd(rb.GetVelocity(), VGet(0, rb.GetJumpPower(), 0)));
			}
			
			rb.AddForce(VScale(moveDir, rb.GetMoveSpeed()));

		}
	}

}
