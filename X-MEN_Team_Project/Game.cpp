#include "Game.h"
#include<Library.h>
#include<Input.h>

#include<ImguiManager.h>



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
	//MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	//MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);
	//MelLib::SceneManager::GetInstance()->SetStartScene(new Play());

}


void Game::Finalize()
{
	// ライブラリ終了処理
	MelLib::Library::Finalize();
}
void Game::Update()
{
	// 2022/04/11 add gates

	//MelLib::SceneManager::GetInstance()->Update();
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();
}