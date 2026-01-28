#pragma once
#include<vector>
#include<span>
#include<iostream>

#include"../../../Object/Actor/EntityKind.h"
#include"ContactEventInfo.h"
#include"GameEventType.h"
class ContactRuleTable
{

public:

	std::span<const ContactRule>Query(
		ContactRule rule
	);


private:

};

