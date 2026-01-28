#pragma once
#include "ColliderBase.h"
class ColliderCapsule :
    public ColliderBase
{
public:
	//コンストラクタ
	ColliderCapsule(ColliderInfo& info, float radius,
		 VECTOR&localPosTop, VECTOR&localPosDown,ActorBase*actor);

	//デストラクタ
	virtual ~ColliderCapsule(void);
	//親Transformからの相対位置を取得
	const VECTOR& GetLocalPosTop() const { return localPosTop_; }
	const VECTOR& GetLocalPosDown() const { return localPosDown_; }
	//半径
	float GetRadius(void) const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }
	//高さ
	float GetHeight(void) const;

	// ワールド座標を取得
	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;
	// カプセルの中心座標
	VECTOR GetCenter(void) const;
protected:
	// デバッグ描画
	void DrawDebug(int color) override;

private:
	//カプセルの上端のローカル位置
	VECTOR localPosTop_;
	//カプセルの下端のローカル位置
	VECTOR localPosDown_;

	//半径
	float radius_;
};

