#include<DxLib.h>
#include "GameScene.h"

#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"


#include"../Manager/System/PhysicsSystem/PhysicsSystem.h"
#include"../Manager/System/MoveInputSystem/MoveInputSystem.h"

#include"../Object/Actor/Component/ComponentBase.h"
#include"../Object/Actor/Component/RigidBodyComponent/RigidBody.h"
#include"../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"

#include"../Object/Actor/ActorBase.h"

#include"../Object/Actor/Shape/Box.h"

#include"../Object/Actor/Camera/Camera.h"

#include"../Object/Actor/Floor/Floor.h"

GameScene::GameScene(void):
	SceneBase()
{
}

GameScene::~GameScene(void)
{
}

template<typename T>
std::vector<std::shared_ptr<T>> ObjSearch(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
	static_assert(std::is_base_of_v<ActorBase, T>);

	std::vector<std::shared_ptr<T>> out;
	out.reserve(objects.size());

	for (const auto& obj : objects)
	{
		if (!obj) continue;

		if (auto casted = std::dynamic_pointer_cast<T>(obj))
		{
			out.push_back(casted); 
		}
	}
	return out;
}
void GameScene::Load(void)  
{  
	// オブジェクト生成  
	std::shared_ptr<Box>box = std::make_shared<Box>();
	actors_.push_back(box);
	box->AddComponent(std::make_shared<PlayerInputComponent>(
			KEY_INPUT_W, KEY_INPUT_S,
			KEY_INPUT_A, KEY_INPUT_D,
			KEY_INPUT_Q, KEY_INPUT_E));
	auto rb = std::make_shared<RigidBody>();
	rb->SetBodyType(RigidBody::BodyType::DYNAMIC);
	rb->SetMass(10.0f);
	rb->SetUseGravity(true);
	rb->SetMoveSpeed(10);
	rb->SetJumpPower(15);
	box->AddComponent(rb);

	std::shared_ptr<Floor>floor = std::make_shared<Floor>();
	rb = std::make_shared<RigidBody>();
	rb->SetBodyType(RigidBody::BodyType::STATIC);
	rb->SetBodyType(RigidBody::BodyType::STATIC);
	rb->SetMass(0.0f);
	floor->AddComponent(rb);
	actors_.push_back(floor);

	// カメラ生成  
	auto camera = std::make_shared<Camera>();  
	actors_.push_back(camera);  
}

void GameScene::Init(void)
{

	for (auto& actor : actors_)
	{
		actor->Init();
		actor->SetEntityId(EntityId);
		EntityId++;
	}
}

void GameScene::Update(void)
{

	for (auto& actor : actors_)
	{
		actor->Update();
	}
}

void GameScene::Draw(void)
{
	DrawString(0, 0, "game", 0xffffff);
	for (auto& actor : actors_)
	{
		actor->Draw();
		for (const auto& [shape, collider] : actor->GetOwnColliders())
		{
			collider->Draw();
		}
	}
}

void GameScene::Release(void)
{
	for (auto& actor : actors_)
	{
		actor->Release();
	}
	actors_.clear();
}
