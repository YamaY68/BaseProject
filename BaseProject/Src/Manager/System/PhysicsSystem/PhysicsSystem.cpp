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
		if (!obj->HasComponent<RigidBody>())continue;
		auto& rigidbody = obj->GetComponent<RigidBody>();

		if (rigidbody.GetInverseMass() == 0.0f)continue;

		//重力の影響を受ける
		if (rigidbody.IsUseGravity())
		{
			if (!rigidbody.IsGrounded())
			{
				rigidbody.AddForce(
					VScale(GRAVITY, rigidbody.GetMass()));
			}
			else rigidbody.SetVelocity({
				rigidbody.GetVelocity().x,0,rigidbody.GetVelocity().z }
				);
		}

		//速度の更新
		VECTOR acceleration = VScale(rigidbody.GetForce(), rigidbody.GetInverseMass());

		//速度 = 速度 + 加速度 * dt
		VECTOR newVelocity = VAdd(
			rigidbody.GetVelocity(),
			VScale(acceleration, dt)
		);

		//減速
		if (rigidbody.IsGrounded())
		{
			newVelocity.x *= 0.8f;
			newVelocity.z *= 0.8f;
		}

		rigidbody.SetVelocity(newVelocity);

		//位置の更新
		VECTOR newPosition = VAdd(
		obj->GetTransform().pos,
			VScale(rigidbody.GetVelocity(), dt));

		obj->GetTransform().prevPos = newPosition;

		//力の初期化
		rigidbody.ClearForce();
	}
}
