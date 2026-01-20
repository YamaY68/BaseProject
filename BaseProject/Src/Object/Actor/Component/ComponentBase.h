#pragma once

class ComponentBase 
{
public:
	virtual ~ComponentBase() = default;
	virtual void OnAttach(class ActorBase* owner) {};
protected:
};