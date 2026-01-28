#include "GameContactSystem.h"
#include"ContactRuleTable.h"

GameContactSystem::GameContactSystem()
{
}

GameContactSystem::~GameContactSystem()
{
}

void GameContactSystem::Update(void)
{
	//接触開始イベントの処理
	for (auto& event : contactSystem_.GetContactEvent())
	{
		Procese(event);
	}

	Clear();
}

void GameContactSystem::Clear(void)
{
	contactSystem_.Clear();
}

void GameContactSystem::Procese(ContactRule rule)
{
	if (rule.contactEvent_.type_ == ContactEventInfo::Type::BEGIN)
	{
		//接触開始時の処理
		int a = 0;
	}
	else if (rule.contactEvent_.type_ == ContactEventInfo::Type::END)
	{
		//接触終了時の処理
	}
}
