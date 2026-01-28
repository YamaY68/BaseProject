#pragma once
#include<cstdint>
#include"GameEventType.h"
using EntityID = std::uint32_t;


struct ContactEventInfo
{
	enum class Type
	{
		BEGIN,
		END,
	};

	Type type_;
	EntityID a;
	EntityID b;
};

struct ContactRule
{
	ContactEventInfo contactEvent_;
	GameEventType eventType_;
};

