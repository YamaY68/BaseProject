#include "TitleScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Resource.h"
#include "../Object/Actor/Camera/Camera.h"
#include"../Common/Quaternion.h"
#include"../Object/Common/AnimationController.h"
TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::TITLE_PUSH).handleId_;
	bigPlanet_.SetModel(
		resMng_.Load(ResourceManager::SRC::PLANET).handleId_);
	smallPlanet_.SetModel(
		resMng_.Load(ResourceManager::SRC::SMALL_PLANET).handleId_);
	player_.SetModel(
		resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);
}

void TitleScene::Init(void)
{
	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	bigPlanet_.scl = AsoUtility::VECTOR_ONE;
	bigPlanet_.quaRot = Quaternion::Identity();
	bigPlanet_.quaRotLocal = Quaternion::Identity();
	bigPlanet_.pos = AsoUtility::VECTOR_ZERO;
	bigPlanet_.Update();

	smallPlanet_.scl = VGet(0.7, 0.7, 0.7);
	smallPlanet_.quaRot = Quaternion::Identity();
	smallPlanet_.quaRotLocal = Quaternion::Identity();
	smallPlanet_.pos = { -250.0f, -100.0f, -100.0f };
	smallPlanet_.Update();

	player_.scl = VGet(0.4f, 0.4f, 0.4f);
	player_.quaRot= Quaternion::Identity();
	player_.quaRotLocal = Quaternion::Identity();
	player_.pos={ -250.0f, -32.0f, -105.0f };
	player_.Update();

	animCtr_ = new AnimationController(player_.modelId);
	animCtr_->Add(0, 20.0f, Application::PATH_MODEL + "Player/Run.mv1");
}

void TitleScene::Update(void)
{

	bigPlanet_.pos.z -= 0.1f;
	bigPlanet_.quaRot = Quaternion::Mult(bigPlanet_.quaRot,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(1),
			VGet(0.5,1,0.3)));
	bigPlanet_.Update();

	smallPlanet_.quaRot = Quaternion::Mult(smallPlanet_.quaRot,
		Quaternion::AngleAxis(AsoUtility::Deg2RadF(-0.5),
			AsoUtility::AXIS_Z));
	smallPlanet_.Update();


	animCtr_->Update();
	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{
	DrawString(0, 0, "title", 0xffffff);
	MV1DrawModel(bigPlanet_.modelId);
	MV1DrawModel(smallPlanet_.modelId);
	MV1DrawModel(player_.modelId);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgTitle_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y * 0.75, 1, 0, imgPush_, true);
	
}

void TitleScene::Release(void)
{
	animCtr_->Release();
}
