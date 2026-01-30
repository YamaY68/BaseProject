#include<DxLib.h>
#include "GameScene.h"

#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"



#include"../Object/Actor/Component/ComponentBase.h"
#include"../Object/Actor/Component/RigidBodyComponent/RigidBody.h"
#include"../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"

#include"../Object/Actor/ActorBase.h"

#include"../Object/Actor/Shape/Box.h"
#include"../Object/Actor/Shape/Capsule.h"
#include"../Object/Actor/Shape/Sphere.h"

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
	auto rb = std::make_shared<RigidBody>();
	auto playerInput = std::make_shared<PlayerInputComponent>();

	std::shared_ptr<Floor>floor = std::make_shared<Floor>();
	rb = std::make_shared<RigidBody>();
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
	auto onBeginContact = [this](uint32_t a, uint32_t b)
		{
			contactSystem_.OnBeginContact(a, b, CollisionResult{});
		};
	auto onEndContact = [this](uint32_t a, uint32_t b)
		{
			contactSystem_.OnEndContact(a, b, CollisionResult{});
		};

	collisionSystem_.SetContactCallbacks(
		static_cast<ContactCallback>(onBeginContact),
		static_cast<ContactCallback>(onEndContact)
	);

	for (auto& actor : actors_)
	{
		actor->Init();
		actor->SetEntityId(EntityId);
		collisionSystem_.AddCollider(actor->GetOwnColliders());
		EntityId++;
	}
}

void GameScene::Update(void)
{

	for (auto& actor : actors_)
	{
		actor->Update();

	}
	moveInputSystem_.Update(actors_);

	
	for(int i=0;i<3;i++)
	{
 	physicsSystem_.Update(actors_);
	collisionSystem_.Update();
	physicsSystem_.Resolve(collisionSystem_.GetMainfolds());
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
