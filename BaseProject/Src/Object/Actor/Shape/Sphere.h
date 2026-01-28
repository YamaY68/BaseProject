#pragma once
#include "ShapeBase.h"
class Sphere :
    public ShapeBase
{
public:
	Sphere(void);
	Sphere(float radius);
	~Sphere(void) override;

private:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;
private:
	void InitCollider(void)override;

private:
	float radius_ = 1.0f;
};

