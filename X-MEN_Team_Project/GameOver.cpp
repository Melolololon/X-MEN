#include "GameOver.h"
#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void GameOver::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)

	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応

	//スプライト
	gameOverFont.Create(MelLib::Color(0, 0, 255, 255));
	gameOverFont.SetScale(MelLib::Vector2(600, 200));
	gameOverFont.SetPosition(MelLib::Vector2(300, 300));
	//// テストオブジェクト追加
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

}

void GameOver::Update()
{
	// マネージャーの更新
	// オブジェクトの更新処理、判定処理、削除処理が行われる
	MelLib::GameObjectManager::GetInstance()->Update();

	// Aキーで現在のシーンを終了して次のシーンへ
	// 今は次のシーンに今と同じシーンをセットしているため、位置がリセットされるだけ
	if (MelLib::Input::KeyTrigger(DIK_SPACE)|| MelLib::Input::PadButtonTrigger(MelLib::PadButton::A))isEnd = true;
}

void GameOver::Draw()
{
	//スプライト
	gameOverFont.Draw();
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
	// 例 return new Title();
	return new GamePlay();
}
