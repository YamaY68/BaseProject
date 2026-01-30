#include "CollisionSystem.h"
#include<algorithm>
#include"../../../Object/Actor/ActorBase.h"
CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::AddCollider(const std::map<int,std::shared_ptr<ColliderBase>>& colliders)
{
	for (auto& c : colliders)
	{
		colliders_.push_back(c.second);
	}
}

void CollisionSystem::ClearColliders()
{
	colliders_.clear();
}

void CollisionSystem::Update(void)
{
	GetActiveColliders();

	std::vector<std::pair<std::size_t, std::size_t>> currentPairs;

	for (size_t i = 0; i < activeColliders_.size(); i++)
	{
		for (size_t j = i + 1; j < activeColliders_.size(); j++)
		{
			auto& a = activeColliders_[i];
			auto& b = activeColliders_[j];

			// レイヤーマスクの判定
			if (!ColliderBase::IsLayerMatch(b->GetColliderInfo().layer, a->GetColliderInfo().mask) &&
				!ColliderBase::IsLayerMatch(a->GetColliderInfo().layer, b->GetColliderInfo().mask)) continue;

			// 1. 形状の順序をチェック（GetCollisionPairTypeの内部ロジックと合わせる）
			SHAPE shapeA = a->GetColliderInfo().shape;
			SHAPE shapeB = b->GetColliderInfo().shape;

			auto finalA = a;
			auto finalB = b;
			bool swapped = false;

			// 形状IDが大きい方を後ろにするように揃える
			if (shapeA > shapeB) {
				std::swap(finalA, finalB);
				swapped = true;
			}

			// 2. 衝突判定の実行（揃えた引数を渡す）
			CollisionPairType pairType = CollisionLogic::GetCollisionPairType(
				finalA->GetColliderInfo().shape,
				finalB->GetColliderInfo().shape);

			CollisionResult result = CollisionLogic::DispatchCollision(pairType, finalA, finalB);

			// 当たっていない場合はスキップ
			if (!result.isHit || result.penetration < PENETRATION_ALLOWANCE) continue;

			// 3. 重要：入れ替えて判定した場合は、法線を「本来のAからBへの向き」に反転させる
			if (swapped) {
				result.normal = VScale(result.normal, -1.0f);
			}

			// 当たっているペアを保存
			auto idA = a->GetFollowActor()->GetEntityId();
			auto idB = b->GetFollowActor()->GetEntityId();
			currentPairs.push_back({ (std::min)(idA, idB), (std::max)(idA, idB) });

			// Solverには「本来のa, b」の順で結果を渡す
			solver.push_back(CollisionSolver{ a->GetFollowActor(), b->GetFollowActor(), result });
		}
	}

	// --- 以降、DiffPairsとコールバック処理（変更なし） ---
	std::vector<std::pair<std::size_t, std::size_t>> begins, ends;
	DiffPairs(currentPairs, prevPairs_, begins, ends);

	if (onBegin_) {
		for (auto [idA, idB] : begins) onBegin_(idA, idB);
	}
	if (onEnd_) {
		for (auto [idA, idB] : ends) onEnd_(idA, idB);
	}

	prevPairs_ = std::move(currentPairs);
}

void CollisionSystem::GetActiveColliders(void)
{
	activeColliders_.clear();
	for (auto& weakCollider : colliders_)
	{
		if (auto collider = weakCollider.lock())
		{
			if(collider->GetColliderInfo().isActive)
			activeColliders_.push_back(collider);
		}
	}
}



void CollisionSystem::DiffPairs(pair& currentPairs, pair& prevPairs, pair& beginPairs, pair& endPairs)
{	//正規化関数
	auto norm = [](auto& v)
		{
			//ソート
			std::sort(v.begin(), v.end());
			//削除
			v.erase(
				//重複している後ろを前に詰める
				std::unique(v.begin(), v.end()),
				v.end());
	};

	//正規化
	norm(currentPairs);
	norm(prevPairs);

	beginPairs.clear();
	endPairs.clear();

	std::size_t a = 0, b = 0;

	while (a < currentPairs.size() && b < prevPairs.size())
	{
		//今フレームのペアが前フレームのペアより大きい場合
		if (currentPairs[a] < prevPairs[b])
			//新規ペア
		{
			beginPairs.push_back(currentPairs[a]); ++a;
		}
		//前フレームのペアが前フレームのペアより大きい場合
		else if (prevPairs[b] < currentPairs[a])
			//消失ペア
		{
			endPairs.push_back(prevPairs[b]); ++b;
		}
		//同じ場合
		//変化なし
		else
		{
			++a; ++b;
		}
	}
	//片方だけ残ってる分を処理
	//今フレーム分
	while (a < currentPairs.size())beginPairs.push_back(currentPairs[a++]);
	//前フレーム分
	while (b < prevPairs.size())endPairs.push_back(prevPairs[b++]);


}
