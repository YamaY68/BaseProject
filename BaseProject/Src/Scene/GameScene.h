#pragma once
#include "SceneBase.h"

#include <vector>
#include <memory>
#include"../Manager/System/CollisionSystem/CollisionSystem.h"
#include"../Manager/System/ContactSystem/ContactSystem.h"
#include"../Manager/System/MoveInputSystem/MoveInputSystem.h"
//#include"../Manager/System/ContactSystem/GameContactSystem.h"
#include"../Manager/System/PhysicsSystem/PhysicsSystem.h"

class ActorBase;

class GameScene :
    public SceneBase
{
public:
	GameScene(void);
	~GameScene(void)override;

	void Load(void)override;


	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;
private:
	std::vector<std::shared_ptr<ActorBase>> actors_;
	//// 衝突判定システム
	CollisionSystem collisionSystem_;
	ContactSystem contactSystem_;
	//GameContactSystem gameContactSystem_;

	MoveInputSystem moveInputSystem_;
	PhysicsSystem physicsSystem_;


	int EntityId = 0;
};

