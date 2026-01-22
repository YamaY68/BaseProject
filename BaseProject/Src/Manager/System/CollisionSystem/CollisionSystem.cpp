#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::AddCollider(const std::map<int, std::shared_ptr<ColliderBase>>& colliders)
{
}

void CollisionSystem::ClearColliders()
{
}

void CollisionSystem::Update(void)
{
}

void CollisionSystem::CollisionCheck(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
}

void CollisionSystem::GetActiveColliders(void)
{
}

void CollisionSystem::BindEntityId(std::size_t index, std::uint32_t entityId)
{
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
