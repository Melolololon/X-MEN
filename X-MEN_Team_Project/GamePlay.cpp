#include "GamePlay.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

void GamePlay::Initialize()
{
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)
	fieldObjectManager = FieldObjectManager::GetInstance();

	// オブジェクトのメモリ確保
	pPlayer = std::make_shared<Player>();
	barrier = std::make_shared<NormalBarrier>();

	pPlayer.get()->SetNormalBarrier(barrier);

	pFollowEnemy = std::make_shared<FollowEnemy>();
	pEnemyBarrier = std::make_shared<EnemyBarrier>();
	pEnemyBarrier.get()->IsOpen();
	pBarrierEnemy = std::make_shared<BarrierEnemy>();
	pBarrierEnemy.get()->SetBarrier(pEnemyBarrier);

	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応
	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	//バリアのテスト
	MelLib::GameObjectManager::GetInstance()->AddObject(barrier);

	// テストオブジェクト追加
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

	// 敵のテスト
	MelLib::GameObjectManager::GetInstance()->AddObject(pFollowEnemy);
	MelLib::GameObjectManager::GetInstance()->AddObject(pBarrierEnemy);
	MelLib::GameObjectManager::GetInstance()->AddObject(pEnemyBarrier);


	fieldObjectManager->Initialize();
}

void GamePlay::Update()
{
	// マネージャーの更新
	// オブジェクトの更新処理、判定処理、削除処理が行われる
	MelLib::GameObjectManager::GetInstance()->Update();

	// 敵のテスト
	pFollowEnemy.get()->SetPlayerDir(pPlayer.get()->GetPosition());
	pBarrierEnemy.get()->SetBallDir(pPlayer.get()->GetBallPos());

	// Aキーで現在のシーンを終了して次のシーンへ
	// 今は次のシーンに今と同じシーンをセットしているため、位置がリセットされるだけ
	if (MelLib::Input::KeyTrigger(DIK_I))isEnd = true;
}

void GamePlay::Draw()
{
	// 描画
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void GamePlay::Finalize()
{
	// 終了処理
	fieldObjectManager->Finalize();

	// 全削除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// 次のシーンのポインタを返す
	// 例 return new Title();
	return new GamePlay();
}
