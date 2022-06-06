#include "GamePlay.h"
#include"Clear.h"
#include"GameOver.h"

#include<GameObjectManager.h>
#include<Input.h>

#include"TestObject.h"

#include"Enemy/EnemyManager.h"

#include"Particle/PopUpParticleManager.h"

#include"GameManager.h"

void GamePlay::Initialize()
{
	MelLib::ModelData::Load("Resources/Model/Dome/dome.obj", true, "domeObj");
	// 初期化処理
	// 必ずコンストラクタではなくここに初期化処理を書く(設計上の都合で)
	fieldObjectManager = FieldObjectManager::GetInstance();

	// オブジェクトのメモリ確保
	pPlayer = std::make_shared<Player>();
	pBall = std::make_shared<Ball>();
	//pBall->SetPosition(MelLib::Vector3(5, 0, -10));
	barrier = std::make_shared<NormalBarrier>();

	pPlayer.get()->SetNormalBarrier(barrier);
	pPlayer.get()->SetBall(pBall);

	//pFollowEnemy = std::make_shared<FollowEnemy>();
	//pEnemyBarrier = std::make_shared<EnemyBarrier>();
	//pEnemyBarrier.get()->OpenBarrier();
	//pBarrierEnemy = std::make_shared<BarrierEnemy>();
	//pBarrierEnemy.get()->SetBarrier(pEnemyBarrier);

	// 管理クラスにオブジェクトを追加
	// ObjectManagerはshared_ptrのみ対応
	MelLib::GameObjectManager::GetInstance()->AddObject(pBall);

	MelLib::GameObjectManager::GetInstance()->AddObject(pPlayer);
	//バリアのテスト
	MelLib::GameObjectManager::GetInstance()->AddObject(barrier);

	//// テストオブジェクト追加
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>(MelLib::Vector3(0, 0, 0)));

	// 敵のテスト
	//MelLib::GameObjectManager::GetInstance()->AddObject(pFollowEnemy);
	//MelLib::GameObjectManager::GetInstance()->AddObject(pBarrierEnemy);
	//MelLib::GameObjectManager::GetInstance()->AddObject(pEnemyBarrier);

	

	//ポップアップパーティクル
	PopUpParticleManager::GetInstance()->Initialize();

	EnemyManager::GetInstance()->Initialize();

	fieldObjectManager->Initialize();

	std::shared_ptr<UltimateSkill> ultimateSkill = std::make_shared<UltimateSkill>();
	dome = std::make_shared<Dome>();
	MelLib::GameObjectManager::GetInstance()->AddObject(dome);
	ultimateSkill.get()->SetDome(dome);
	pPlayer.get()->SetUltimateSkill(ultimateSkill);

	// GameManagerのテスト
	GameManager::GetInstance()->Initialize();

	nextScene = NextScene::PLAY;
}

void GamePlay::Update()
{

	if (!GameManager::GetInstance()->IsHitStop())
	{
		// マネージャーの更新
		// オブジェクトの更新処理、判定処理、削除処理が行われる
		MelLib::GameObjectManager::GetInstance()->Update();

		// 必殺技を使ったあとにゲーム内時間をゆっくりにする
		if (dome.get()->IsEndTrigger())GameManager::GetInstance()->SetGameTime(0.1f);
		if (dome.get()->IsPostProcessEndTrigger())GameManager::GetInstance()->SetDefaultGameTime();

		// 敵のテスト
		//pFollowEnemy.get()->SetPlayerPos(pPlayer.get()->GetPosition());
		//pFollowEnemy.get()->SetPlayerDir(pPlayer.get()->GetPosition());

		EnemyManager::GetInstance()->SetPlayerPos(pPlayer.get()->GetPosition());
		EnemyManager::GetInstance()->Update();

		//ボール取得(とりあえず最初に見つかった1つ)
		//for (const auto& v : MelLib::GameObjectManager::GetInstance()->GetRefGameObject()) {
		//	if (typeid(*v) == typeid(Ball)) {
		//		pBarrierEnemy.get()->SetBallDir(v->GetPosition());
		//		break;
		//	}
		//}

		//ポップパーティクル
		PopUpParticleManager::GetInstance()->Update();
		if (MelLib::Input::KeyTrigger(DIK_6))
		{
			PopUpParticleManager::GetInstance()->SetFire(MelLib::Vector3());

		}

		// 確認用
		if (MelLib::Input::KeyTrigger(DIK_0)) GameManager::GetInstance()->SetHitStop(true);

		// ゲームクリア条件 
		// 敵の総数が0以下になったときに
		if (EnemyManager::GetInstance()->GetEnemyCount() <= 0)GameManager::GetInstance()->SetCanGameClear(true);
		
		// ゲームオーバー条件
		if (pPlayer.get()->GetHp() <= 0)GameManager::GetInstance()->SetCanGameOver(true);

		GameManager::GetInstance()->UpdateChangeGameTime();
	}
	else
	{
		GameManager::GetInstance()->Update();
	}


	// Aキーで現在のシーンを終了して次のシーンへ
	// 今は次のシーンに今と同じシーンをセットしているため、位置がリセットされるだけ
	//if (MelLib::Input::KeyTrigger(DIK_I))isEnd = true;
	if (MelLib::Input::KeyTrigger(DIK_L))nextScene = NextScene::CLEAR;
	if (MelLib::Input::KeyTrigger(DIK_P) || pPlayer.get()->GetHp() <= 0)nextScene = NextScene::GAMEOVER;

	if (GameManager::GetInstance()->GetCanGameClear())nextScene = NextScene::CLEAR;
	else if (GameManager::GetInstance()->GetCanGameOver())nextScene = NextScene::GAMEOVER;


	if (nextScene != NextScene::PLAY)
	{
		isEnd = true;
	}
}

void GamePlay::Draw()
{
	// 描画
	MelLib::GameObjectManager::GetInstance()->Draw();
	//ポップアップパーティクル
	PopUpParticleManager::GetInstance()->Draw();
	
	// 半透明なので描画
	pBall->DrawTrajectories();

}

void GamePlay::Finalize()
{
	// 終了処理
	fieldObjectManager->Finalize();

	// 全削除
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();

	EnemyManager::GetInstance()->Destroy();
}

MelLib::Scene* GamePlay::GetNextScene()
{
	// 次のシーンのポインタを返す
	switch (nextScene)
	{
	case NextScene::CLEAR:
		return new Clear();
		nextScene = NextScene::PLAY;
		break;
	case NextScene::GAMEOVER:
		return new GameOver();
		nextScene = NextScene::PLAY;
		break;
	}
}
