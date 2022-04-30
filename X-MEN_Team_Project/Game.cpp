#include "Game.h"
#include<Library.h>
#include<Input.h>

#include<ModelObject.h>

#include<Camera.h>

#include<ImguiManager.h>
#include<SceneManager.h>
#include<GameObjectManager.h>

#include"GamePlay.h"
#include "InputDeviceManager.h"

Game::Game() {}

Game::~Game() {}

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{
	// リリースビルド時にImGuiを描画するか指定
	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);

	// 初期化
	Initialize();

	// ゲームループ(書き換える必要なし!)
	while (1)
	{
		MelLib::Library::LoopStartProcess();

		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		if (MelLib::Library::GetIsEnd())break;


		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	// 終了
	Finalize();
}

void Game::Initialize()
{
	// ライブラリ初期化(横,縦,色,名前)
	MelLib::Library::Initialize(1280, 720, MelLib::Color(30, 30, 160, 255), L"Game");
	// 60フレーム固定
	MelLib::Library::SetFramesPerSecond60(true);


	// マネージャーの設定
	// 	 
	//MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	// メモリ先行確保
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);
	// 初期シーンセット
	MelLib::SceneManager::GetInstance()->SetStartScene(new GamePlay());

	// 箱作成
	// デフォルトだと1,1,1の白で生成される
	// 形状たくさんあるけど今はBOXとBOARDのみ
	// 長いから変更する可能性あり(ShapeType3Dだけで指定できるようにする予定)
	//testModel.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// メインカメラを取得(Get関数に何も指定しないとメインカメラを取得する)
	// SetRotateCriteriaPositionで回転基準をセット
	//(初期設定だとFSPカメラのため、実質カメラの座標。TPS視点の時はこれでセットした数値が注視点の座標になる)
	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,10,-10));

	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(45, 0, 0));

	// 2022/05/01 スズキ追加
	InputDeviceManager::GetInstance()->Initialize();
}


void Game::Finalize()
{
	// ライブラリ終了処理
	MelLib::Library::Finalize();
}
void Game::Update()
{
	// 2022/04/11 add gates
	// 2022/04/11 add GUTTYman
	// 2022/04/11 add ShunNedachi

	// 2022/05/01 スズキ追加
	InputDeviceManager::GetInstance()->Update();

	MelLib::SceneManager::GetInstance()->Update();
	
#pragma region キーボードと色変えのチュートリアル

	//// 毎フレームY軸基準で3度回転
	//testModel.SetAngle(testModel.GetAngle() + MelLib::Vector3(0, 3, 0));

	//// カメラの操作
	//// キーボードで入力して回転
	//if (MelLib::Input::KeyState(DIK_LEFT)) {
	//	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, MelLib::Camera::Get()->GetAngle().y - 2, 0));
	//}
	//if (MelLib::Input::KeyState(DIK_RIGHT)) {
	//	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, MelLib::Camera::Get()->GetAngle().y + 2, 0));
	//}

	//// 色の乗算
	//// マテリアルを生成して割り当てることもできるが、簡単な色変えならこちらでOK
	//// Color(rgb,a)
	//// 0～255で指定
	//// ParToUCharは0%～100%を0～255に変換
	////
	//testModel.SetMulColor(MelLib::Color(255,255,255, MelLib::Color::ParToUChar(90)));
#pragma endregion

}

void Game::Draw()
{
	MelLib::SceneManager::GetInstance()->Draw();

	// 描画
	//testModel.Draw();
}