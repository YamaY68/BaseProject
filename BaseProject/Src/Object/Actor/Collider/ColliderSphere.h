#pragma once
#include <DxLib.h>
#include"ColliderBase.h"

class ColliderSphere : public ColliderBase
{
public:
	//デフォルトコンストラクタ
	ColliderSphere(void);
	//コンストラクタ
	ColliderSphere(ColliderInfo& info, float radius);
	//色変え用コンストラクタ
	ColliderSphere(ColliderInfo& info, float radius, int color);

	//デストラクタ
	virtual ~ColliderSphere(void);

	//半径
	float GetRadius() const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

protected:
	// デバッグ描画
	void DrawDebug(int color) override;

private:
	//半径
	float radius_;
};