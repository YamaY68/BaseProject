#pragma once
#include<DxLib.h>
#include<vector>
#include<cstdint>
#include<memory>
#include<map>
#include<functional>
#include"../../../Object/Actor/Collider/ColliderBase.h"
#include"CollisionLogic.h"

using ContactCallback = std::function<void(std::uint32_t, std::uint32_t)>;
//フレームごとの衝突情報を保持するための型
using pair = std::vector<std::pair<std::size_t, std::size_t>>;

class CollisionSystem
{
public:
	struct CollisionManifold {
		ActorBase* actorA;
		ActorBase* actorB;
		CollisionResult result; 
	};

	CollisionSystem();
	~CollisionSystem();

	void AddCollider(const std::map<int, std::shared_ptr<ColliderBase>>& colliders);
	void ClearColliders();

	void Update(void);

	//コールバック設定
	void SetContactCallbacks(ContactCallback onBegin, ContactCallback onEnd)
	{
		onBegin_ = onBegin;
		onEnd_ = onEnd;
	}

private:
	void BindEntityId(std::size_t index, std::uint32_t entityId);
	void GetActiveColliders(void);


	/// @brief 新しく当たったもの、離れたものを抽出
/// @param currentPairs	今フレームの接触ペア
/// @param prevPairs 前フレームの接触ペア
/// @param beginPairs 新しく発生したペア
/// @param endPairs 消えたペア
	void DiffPairs(pair& currentPairs, pair& prevPairs, pair& beginPairs, pair& endPairs);
private:
	//衝突回数の上限
	static constexpr unsigned char MAX_COLLISION_COUNT = 3;
	//めり込み許容値
	static constexpr float PENETRATION_ALLOWANCE = 0.05f;

	std::vector<std::weak_ptr<ColliderBase>>colliders_;
	std::vector<std::shared_ptr<ColliderBase>>activeColliders_;
	std::vector<std::int32_t>entityId_;

	pair prevPairs_;

	ContactCallback onBegin_;
	ContactCallback onEnd_;

	//衝突情報のメインフォールド
	std::vector<CollisionManifold>mainfolds_;
};

