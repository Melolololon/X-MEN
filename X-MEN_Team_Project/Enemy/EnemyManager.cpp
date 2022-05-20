#include "EnemyManager.h"
#include<GameObjectManager.h>
#include<memory>
#include<time.h>
#include<random>

// 静的な実体
EnemyManager* EnemyManager::instance;

EnemyManager* EnemyManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new EnemyManager();
	}
	
	return instance;
}

void EnemyManager::Initialize()
{
	// 追従する敵のオブジェクトの設置
	PopFollowEnemyInitialize();
	// バリア持ちの敵のオブジェクトの設置
	PopBarrierEnemyInitialize();

	// 出現時間のタイマーセット
	timerStart = clock();

	// 初期化
	enemyCount = 0;
}

void EnemyManager::Update()
{
	// 時間での敵の出現
	PopEnemyTime();

	CheckEnemyDead(followEnemies);
	CheckEnemyDead(barrierEnemies);
}

void EnemyManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

}

void EnemyManager::SetPlayerPos(const MelLib::Vector3& pos)
{
	// 追従する敵にプレイヤーの座標を持ってくる
	for (auto x : followEnemies)
	{
		x.get()->SetPlayerDir(pos);
		x.get()->SetPlayerPos(pos);
	}
}

int EnemyManager::GetEnemyCount() const
{
	return enemyCount;
}

void EnemyManager::PopFollowEnemyInitialize()
{
	std::random_device random;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(-10, 10);


	for (int i = 0; i < EnemyManage::SPAWN_FOLLOW_ENEMY_NUM; i++)
	{
		std::shared_ptr<FollowEnemy> temp = std::make_shared<FollowEnemy>();
		// 見た目の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);

		// 後で位置のランダム配置


		temp.get()->SetPosition(MelLib::Vector3(dist(engine), 0, dist(engine)));


		followEnemies.push_back(temp);

		enemyCount++;
	}
}

void EnemyManager::PopBarrierEnemyInitialize()
{

	// バリア持ちの敵を生成
	for (int i = 0; i < EnemyManage::SPAWN_BARRIER_ENEMY_NUM; i++)
	{
		std::shared_ptr<BarrierEnemy> temp = std::make_shared<BarrierEnemy>();

		std::shared_ptr<EnemyBarrier> tempBarrier = std::make_shared<EnemyBarrier>();

		// バリア設定
		tempBarrier.get()->OpenBarrier();

		// 見た目の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);
		MelLib::GameObjectManager::GetInstance()->AddObject(tempBarrier);



		// とりあえず0,0,0に設置
		temp.get()->SetPosition(MelLib::Vector3(0, 0, 0));
		temp.get()->SetBarrier(tempBarrier);

		barrierEnemies.push_back(temp);
		enemyBarriers.push_back(tempBarrier);

		enemyCount++;
	}
}

void EnemyManager::PopEnemyTime()
{
	// 一定秒数たったら敵の追加
	// 経過時間の計測
	clock_t timerEnd = clock(); 
	

	double time = (double)(timerEnd - timerStart) / CLOCKS_PER_SEC;

	// 設定した時間よりも経過時間が長かったら
	if (time > EnemyManage::SPAWN_TIME)
	{

		std::shared_ptr<FollowEnemy> followEnemy = std::make_shared<FollowEnemy>();

		//　見た目の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(followEnemy);


		followEnemies.push_back(followEnemy);

		timerStart = clock();

		enemyCount++;
	}

	
}