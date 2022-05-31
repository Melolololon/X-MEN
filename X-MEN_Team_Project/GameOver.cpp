#include "GameOver.h"
#include "GamePlay.h"
#include"Title.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TextWrite.h"
#include"TestObject.h"

void GameOver::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)

	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応

	//GameOverフォント
	MelLib::TextWrite::CreateFontData(L"Arial", 100, "Test");
	MelLib::TextWrite::CreateFontData(L"Arial", 50, "key text");
	
	//// テストオブジェクト追加
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void GameOver::Update()
{
	// マネージャーの更新
	// オブジェクトの更新処理、判定処理、削除処理が行われる
	MelLib::GameObjectManager::GetInstance()->Update();

	// Aキーで現在のシーンを終了してタイトルへ
	// bキーでGamePlayへ移行
	if (MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))nextScene = NextScene::TITLE;
	else if(MelLib::Input::KeyTrigger(DIK_V) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::B))nextScene = NextScene::PLAY;

	if (nextScene != NextScene::GAMEOVER)
	{
		isEnd = true;
	}
}

void GameOver::Draw()
{
	//GameOverフォント
	MelLib::TextWrite::Draw(MelLib::Vector2(400,300), MelLib::Color(0,0,255,255), L"GameOver", "Test");
	MelLib::TextWrite::Draw(MelLib::Vector2(300, 500), MelLib::Color(255, 255, 255, 255), L"SPACE/Aでタイトルへ、V/Bでリトライ", "key text");


	// 描画
	MelLib::GameObjectManager::GetInstance()->Draw();


}

void GameOver::Finalize()
{

	// 全削除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GameOver::GetNextScene()
{
	// 次のシーンのポインタを返す
	switch (nextScene)
	{
	case NextScene::TITLE:
		return new Title();
		break;
	case NextScene::PLAY:
		return new GamePlay();
		break;
	}
}
