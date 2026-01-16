#pragma once
#include<DxLib.h>
#include <chrono>
#include<memory>
#include<list>
#include<map>

#include"../Application.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager
{
public:
	// シングルトン（生成・取得・削除）
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new SceneManager(); } };
	static SceneManager& GetInstance(void) { return *instance_; };
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }
private:
	// 静的インスタンス
	static SceneManager* instance_;
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;
	// デストラクタも同様
	~SceneManager(void) = default;
public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE = -1,
		TITLE,
		GAME,
		GAMEEND,

		MAX,
	};
	
	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(std::shared_ptr<SceneBase>scene);
	void ChangeScene(SCENE_ID scene);

	// シーンを新しく積む
	void PushScene(std::shared_ptr<SceneBase>scene);
	void PushScene(SCENE_ID scene);

	// 最後に追加したシーンを削除する。
	void PopScene(void);

	// 強制的に特定のシーンに飛ぶ。リセットをかけ特定のシーンのみにする。
	void JumpScene(std::shared_ptr<SceneBase>scene);
	void JumpScene(SCENE_ID scene);

	// シーンIDの取得
	SCENE_ID GetSceneID(void) { return sceneId_; }

	

	// デルタタイムの取得
	float GetDeltaTime(void) const {return deltaTime_;}
	// カメラの取得
	Camera* GetCamera(void) const;
private:
	//シーン
	std::list<std::shared_ptr<SceneBase>>scenes_;
	SCENE_ID sceneId_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	// カメラ
	Camera* camera_;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// フェード
	void Fade(void);

	//３Dの初期化処理
	void Init3D(void);

	// 画面揺れ------------------------
	int mainScreen_;
	//int shake_;
	//ShakeKinds shakeKinds_;
	//ShakeSize shakeSize_;
	//Vector2 ShakePoint(void);
	//---------------------------------

};

using SCENE_ID = SceneManager::SCENE_ID;
