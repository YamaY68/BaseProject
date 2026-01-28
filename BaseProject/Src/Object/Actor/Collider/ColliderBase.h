#pragma once  
#include <DxLib.h>  
#include <cstdint>  
#include<initializer_list>
#include"ColliderInfo.h"


class Transform;  
class ActorBase;
class ColliderBase  
{  
public:

   //コンストラクタ
   ColliderBase(ColliderInfo& info, ActorBase*actor);


   //デストラクタ
   virtual ~ColliderBase(void) = default;

   // 初期化  
   virtual void Init(void);  

   // 更新  
   virtual void Update(void);  

   // 描画  
   virtual void Draw(void);  

   // 解放  
   virtual void Release(void);  

	//コライダ情報取得
	const ColliderInfo& GetColliderInfo() const { return colliderInfo_; }

	//親Transform取得
	Transform* GetFollow(void) const { return colliderInfo_.targetTransform; }
	//親Actor取得
	ActorBase* GetFollowActor(void) const { return actor_; }
	//親Actor設定
	void SetFollowActor(ActorBase* actor) { actor_ = actor; }

   // レイヤーマッチ判定
   static bool IsLayerMatch(Layer layerA, uint32_t maskA)  
   {  
       return (maskA & LAYER_BIT(layerA)) != 0;  
   }  
	// マスク作成
   static uint32_t SetMask(std::initializer_list<Layer> layers)  
   {  
       uint32_t mask = 0;  
       for (Layer layer : layers)  
           mask |= LAYER_BIT(layer);  
       return mask;  
   }  
	// ローカル座標から回転後のワールド座標を取得
   VECTOR GetRotPos(const VECTOR& localPos) const;  
protected:  
   // デバッグ描画
   virtual void DrawDebug(int color) {};  

   // レイヤービット取得
   static constexpr uint32_t LAYER_BIT(Layer layer)  
   {  
       return 1 << static_cast<uint32_t>(layer);  
   }  

protected:  
	// コライダ情報
	ColliderInfo colliderInfo_;
	int debugColor_;

private:
	// デバッグ表示の色
	static constexpr int COLOR_VALID = 0xff0000;
	static constexpr int COLOR_INVALID = 0xaaaaaa;

	ActorBase* actor_ = nullptr;
};
