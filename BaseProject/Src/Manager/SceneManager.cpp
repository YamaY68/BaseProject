#include "SceneManager.h"

#include <chrono>
#include <DxLib.h>
#include<EffekseerForDXLib.h>

#include"../Scene/SceneBase.h"

#include "../Common/Fader.h"
#include"ResourceManager.h"

#include"../Manager/Decoration/EffectManager.h"


#include"../Object/Actor/Camera/Camera.h"
#include"../Manager/Generic/Loading.h"

#include"../Scene/TitleScene.h"
#include"../Scene/GameScene.h"
#include"../Scene/EndScene.h"

//#include"../Scene/SceneTitle/SceneTitle.h"
//#include"../Scene/SceneGame/SceneGame.h"
//#include"../Scene/SceneNumber/SceneNumberSelect.h"
//#include"../Scene/SceneRoleSelect/SceneRoleSelect.h"
//#include"../Scene/SceneStageSelect/SceneStageSelect.h"
//#include"../Scene/SceneGameEnd/SceneGameEnd.h"
//#include"../Scene/SceneGameModeSelect/SceneGameModeSelect.h"

SceneManager* SceneManager::instance_ = nullptr;

SceneManager::SceneManager(void) :
	sceneId_(SCENE_ID::NONE),
	deltaTime_(1.0f / 60)
{

}


void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;

	// ロード画面生成
	Loading::GetInstance()->CreateInstance();
	Loading::GetInstance()->Init();
	Loading::GetInstance()->Load();

	// カメラ
	camera_ = new Camera();
	camera_->Init();

	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();

	//３D用の設定
	Init3D();

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 初期シーンの設定
	ChangeScene(SCENE_ID::TITLE);

	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void SceneManager::Update(void)
{	
	//シーンがなければ終了
	if (scenes_.empty()) {return;}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	// ロード中
	if (Loading::GetInstance()->IsLoading())
	{
		// ロード更新
		Loading::GetInstance()->Update();

		// ロードの更新が終了していたら
		if (Loading::GetInstance()->IsLoading() == false)
		{
			// ロード後の初期化
			scenes_.back()->Init();
		}
	}
	// 通常の更新処理
	else
	{
		////ヒットストップ-----------------
		//if (hitStopCounter_ > 0) { hitStopCounter_--; return; }
		////スロー--------------------------
		//if (slowCounter_ > 0) {
		//	slowCounter_--;
		//	if (slowCounter_ % 5 != 0) { return; }
		//}
		////注視点を初期化-----------------
		//zoomPos_ = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };
		//scale_ = 1.0f;
		////--------------------------------

		// 現在のシーンの更新
		scenes_.back()->Update();
	}
		// カメラ更新
		camera_->Update();

}

void SceneManager::Draw(void)
{

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	//エフェクシアの更新
	UpdateEffekseer3D();


	// 描画
	// ロード中ならロード画面を描画
	if (Loading::GetInstance()->IsLoading())
	{
		// ロードの描画
		Loading::GetInstance()->Draw();
	}
	// 通常の更新
	else
	{
		// カメラ設定
		camera_->SetBeforeDraw();
		// 積まれているもの全てを描画する
		for (auto& scene : scenes_)
		{
			scene->Draw();
		}
	}
	//エフェクシアの描画
	DrawEffekseer3D();
	// カメラ描画
	camera_->DrawDebug();

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();



	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1, 0, mainScreen_, true);
}

void SceneManager::Destroy(void)
{

	// シーンの解放
		//全てのシーンの解放・削除
	for (auto& scene : scenes_) { scene->Release(); }
	scenes_.clear();

	//カメラの解放
	camera_->Release();
	delete camera_;

	DeleteGraph(mainScreen_);

	// ロード画面の削除
	Loading::GetInstance()->Release();
	Loading::GetInstance()->DeleteInstance();


	// インスタンスのメモリ解放
	delete instance_;
	instance_ = nullptr;

}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	// シーンが空か？
	if (scenes_.empty())
	{
		//空なので新しく入れる
		scenes_.push_back(scene);
	}
	else
	{
		//末尾のものを新しい物に入れ替える
		ResourceManager::GetInstance().Release();
		scenes_.back()->Release();
		scenes_.back() = scene;
	}

	// 読み込み(非同期)
	Loading::GetInstance()->StartAsyncLoad();
	scenes_.back()->Load();
	Loading::GetInstance()->EndAsyncLoad();
}

void SceneManager::ChangeScene(SCENE_ID scene)
{
	switch (scene)
	{
	case SceneManager::SCENE_ID::TITLE:
		ChangeScene(std::make_shared<TitleScene>());
		break;
	case SceneManager::SCENE_ID::GAME:
		ChangeScene(std::make_shared<GameScene>());
		break;
	case SceneManager::SCENE_ID::GAMEEND:
		ChangeScene(std::make_shared<EndScene>());
		break;
	default:
		break;
	}
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	//新しく積むのでもともと入っている奴はまだ削除されない
	scenes_.push_back(scene);
	scenes_.back()->Load();
	scenes_.back()->Init();
}

void SceneManager::PushScene(SCENE_ID scene)
{

}

void SceneManager::PopScene(void)
{
	//積んであるものを消して、もともとあったものを末尾にする
	if (scenes_.size() > 1)
	{
		scenes_.back()->Release();
		scenes_.pop_back();
	}
}

void SceneManager::JumpScene(std::shared_ptr<SceneBase> scene)
{
	// 全て解放
	for (auto& scene : scenes_) { scene->Release(); }
	scenes_.clear();

	// 新しく積む
	ChangeScene(scene);
}

void SceneManager::JumpScene(SCENE_ID scene)
{
	switch (scene)
	{
	case SceneManager::SCENE_ID::TITLE:
		ChangeScene(std::make_shared<TitleScene>());
		break;
	case SceneManager::SCENE_ID::GAME:
		ChangeScene(std::make_shared<GameScene>());
		break;
	case SceneManager::SCENE_ID::GAMEEND:
		ChangeScene(std::make_shared<EndScene>());
		break;
	default:
		break;
	}
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}




void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::Fade(void)
{
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 139, 139);
	// Zバッファを有効にする
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);
	// バックカリングを有効にする
	SetUseBackCulling(true);
	// ライトを有効にする
	SetUseLighting(true);
	// ディレクショナルライト方向の設定(正規化されていなくても良い)
	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	//// フォグ設定
	//SetFogEnable(true);
	//// フォグの色
	//SetFogColor(100, 100, 100);
	//// フォグを発生させる奥行きの最小、最大距離
	//SetFogStartEnd(500 , 5000 );

}


