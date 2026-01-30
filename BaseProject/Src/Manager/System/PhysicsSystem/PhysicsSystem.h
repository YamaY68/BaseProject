#pragma once
#include<vector>
#include<memory>
#include"../CollisionSystem/CollisionSolver.h"
class ActorBase;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Update(const std::vector<std::shared_ptr<ActorBase>>& objects);
	
	void Resolve(std::vector<CollisionSolver>&solver);

private:
};

