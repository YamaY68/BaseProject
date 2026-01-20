#pragma once
#include<vector>
#include<memory>

class ActorBase;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Update(const std::vector<std::shared_ptr<ActorBase>>& objects);
	

private:
};

