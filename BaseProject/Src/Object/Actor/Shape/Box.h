#pragma once
#include <DxLib.h>
#include "ShapeBase.h"
class Box :
    public ShapeBase
{
public:
	Box(void);
	Box(const VECTOR& halfSize);
	~Box(void) override;

private:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;
private:
	void InitCollider(void)override;

private:
	VECTOR halfSize_;
};

