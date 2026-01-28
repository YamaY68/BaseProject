#pragma once
#include<vector>
#include<map>
#include<memory>
#include<unordered_map>
#include<typeindex>


#include"../Common/Transform.h"
#include"Collider/ColliderBase.h"

class AnimationController;


class ActorBase
{
public:
	ActorBase(void);
	virtual ~ActorBase(void);

	//読み込み
	void Load(void);
	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);

	Transform& GetTransform(void) { return trans_; }

	//エンティティID取得
	size_t GetEntityId(void) const { return entityId_; }
	//エンティティID設定
	void SetEntityId(int id) { entityId_ = id; }

	//自身のコライダーリスト取得
	const std::map<int, std::shared_ptr<ColliderBase>>& GetOwnColliders(void) const { return ownColliders_; }


	//コンポーネント追加
	template<class T>
	ActorBase& AddComponent(std::shared_ptr<T>component);
	//コンポーネントがあるか
	template<class T>
	bool HasComponent(void);
	//コンポーネント取得
	template<class T>
	T& GetComponent(void);

protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void InitCollider(void) {};

	void SetOwnerActor2Colliders(void);

protected:

	AnimationController* animationCOntroller_ = nullptr;
	Transform trans_;
	size_t entityId_ = -1;

	//自身のコライダーリスト
	std::map<int, std::shared_ptr<ColliderBase>>ownColliders_;

	//コンポーネントリスト
	std::unordered_map<std::type_index, std::shared_ptr<void>>components_;

private:



};

template<class T>
inline ActorBase& ActorBase::AddComponent(std::shared_ptr<T> component)
{
	components_[std::type_index(typeid(T))] = component;
	return *this;
}


template<class T>
inline bool ActorBase::HasComponent(void)
{

	return components_.find(std::type_index(typeid(T))) != components_.end();
}

template<class T>
inline T& ActorBase::GetComponent(void)
{
	return *std::static_pointer_cast<T>(components_[std::type_index(typeid(T))]);
}
