#pragma once
#include "ShapeBase.h"
class Capsule :
    public ShapeBase
{
public:
	Capsule(void);
	Capsule(float radius, VECTOR localPosTop, VECTOR localPosDown);
	~Capsule(void) override;

protected:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;
	void InitCollider(void) override;

private:
	float radius_;
	//カプセルの上端のローカル位置
	VECTOR localPosTop_;
	//カプセルの下端のローカル位置
	VECTOR localPosDown_;
};

