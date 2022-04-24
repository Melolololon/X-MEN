#include "GamePlay.h"

#include<GameObjectManager.h>

#include"TestObject.h"

void GamePlay::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)

	// プレイヤーのメモリ確保
	pPlayer = std::make_shared<Player>();

	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応
	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);

	// テストオブジェクト追加
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));
}

void GamePlay::Update()
{
	// マネージャーの更新
	// オブジェクトの更新処理、判定処理、削除処理が行われる
	MelLib::GameObjectManager::GetInstance()->Update();
}

void GamePlay::Draw()
{
	// 描画
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
	// 終了処理

	// 全削除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// 次のシーンのポインタを返す
	// 例 return new Title();
	return nullptr;
}
