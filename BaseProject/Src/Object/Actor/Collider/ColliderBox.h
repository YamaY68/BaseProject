#pragma once
#include "ColliderBase.h"

class ColliderBox : public ColliderBase
{
public:
	// コンストラクタ
	// halfSize : 各軸方向の半サイズ（中心からの距離）
	ColliderBox(ColliderInfo& info,  VECTOR& halfSize,ActorBase*actor);
	// デストラクタ
	virtual ~ColliderBox(void);

	// ボックスの半サイズ取得
	const VECTOR& GetHalfSize(void) const { return halfSize_; }
	void SetHalfSize(const VECTOR& halfSize) { halfSize_ = halfSize; }

	// 各軸ベクトル取得
	VECTOR GetAxisX(void)const { return GetFollow()->GetRight(); }
	VECTOR GetAxisY(void)const { return GetFollow()->GetUp(); }
	VECTOR GetAxisZ(void)const { return GetFollow()->GetForward(); }

	// ローカル座標⇔ワールド座標変換
	VECTOR Local2World(const VECTOR& localPos) const;
	VECTOR World2Local(const VECTOR& worldPos) const;


	//ボックスの各頂点座標取得
	VECTOR GetVertexPos(int index) const;

protected:
	// デバッグ描画
	void DrawDebug(int color) override;

private:
	// ボックスの半サイズ（Half Extents）
	VECTOR halfSize_;
};
