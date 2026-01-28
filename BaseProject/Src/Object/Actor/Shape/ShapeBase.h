#pragma once
#include "../ActorBase.h"
#include<memory>
class ShapeBase :
    public ActorBase
{
public:
	ShapeBase(void);
	~ShapeBase(void) override;

protected:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;
	
	//void OnSupported(void) override;

protected:
	// å`èÛÇÃêF
	int color;
};

