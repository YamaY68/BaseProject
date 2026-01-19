#pragma once
#include<vector>
#include<memory>
class ActorBase;

class MoveInputSystem
{
public:
	MoveInputSystem();
	~MoveInputSystem();

	void Update(const std::vector<std::shared_ptr<ActorBase>>& objects);
};

