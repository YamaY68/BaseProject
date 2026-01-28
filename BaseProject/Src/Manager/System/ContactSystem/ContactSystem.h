#pragma once
#include<cstdint>
#include<map>
#include<set>
#include<vector>

#include"../CollisionSystem/CollisionResult.h"
#include"ContactEventInfo.h"

class ContactSystem
{
public:
	//コリジョンシステムからの接触イベント
	void OnBeginContact(EntityID a, EntityID b, CollisionResult result);
	void OnEndContact(EntityID a, EntityID b, CollisionResult result);

	std::vector<ContactRule> GetContactEvent() const
	{
		return contactRules_;
	}

	void Clear()
	{
		contactRules_.clear();
	}

private:

	//このIDがどのIDたちと当たっているかを格納
	std::map<EntityID, std::set<EntityID>> touching_;

	std::vector<ContactRule>contactRules_;
};

