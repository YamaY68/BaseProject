#pragma once
#include "SceneBase.h"
class EndScene :
    public SceneBase
{

public:

	EndScene(void);
	~EndScene(void)override;

	void Load(void)override;


	// ‰Šú‰»
	void Init(void) override;

	// XV
	void Update(void) override;

	// •`‰æ
	void Draw(void) override;

	// ‰ğ•ú
	void Release(void) override;
private:
};

