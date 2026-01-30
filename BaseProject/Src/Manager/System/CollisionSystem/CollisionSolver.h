#pragma once
#include "../../../Object/Actor/ActorBase.h"
#include"CollisionResult.h"

struct CollisionSolver {
	ActorBase* actorA;
	ActorBase* actorB;
	CollisionResult result;
};