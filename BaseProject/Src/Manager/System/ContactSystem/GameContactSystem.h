#pragma once
#include"ContactSystem.h"

class GameContactSystem
{
public:
	GameContactSystem();
	~GameContactSystem();

	void Update(void);

	void Clear(void);



private:
	// ÚGƒCƒxƒ“ƒg‚Ìˆ—
	void Procese(ContactRule rule);

	ContactSystem contactSystem_;



};

