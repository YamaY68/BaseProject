#pragma once
#include "SceneBase.h"
#include"../Object/Common/Transform.h"

class AnimationController;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	void Load(void)override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:
	int imgTitle_;
	int imgPush_;
	Transform bigPlanet_;
	Transform smallPlanet_;
	Transform player_;
	AnimationController* animCtr_;
};
