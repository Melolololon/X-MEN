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
	// 初期化
	enemyCount = 0;

	// 追従する敵のオブジェクトの設置
	PopFollowEnemyInitialize();
	// バリア持ちの敵のオブジェクトの設置
	PopBarrierEnemyInitialize();

	// 出現時間のタイマーセット
	timerStart = clock();
}

void EnemyManager::Update()
{
	// 時間での敵の出現
	PopEnemyTime();

	// 敵がぶつからない移動をする
	EnemyMoveCancel();

	CheckEnemyDead(followEnemies,false);
	CheckEnemyDead(barrierEnemies,true);
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

	// バリア持ちの敵にプレイヤーの座標を持ってくる
	for (auto x : barrierEnemies)
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
		//temp.get()->SetPosition(MelLib::Vector3(0, 0, 0));
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

void EnemyManager::EnemyMoveCancel()
{
	// 追従する敵の計算
	CalcFollowEnemyMove();

	// バリア持ちの計算
	CalcBarrierEnemyMove();

	// バリアと追従する敵の計算
	CalcFollowToBarrierEnemyMove();
}

void EnemyManager::CalcFollowEnemyMove()
{
	for (int i = 0; i < followEnemies.size(); i++)
	{
		for (int j = 0; j < followEnemies.size(); j++)
		{
			// 同じ敵同士は無視
			if (i != j)
			{
				// posをとってきて距離を計算
				MelLib::Vector3 iEnemyPosition = followEnemies[i].get()->GetPosition();
				MelLib::Vector3 jEnemyPosition = followEnemies[j].get()->GetPosition();

				// 距離を計算
				float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
				float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
				float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
				{
					// ぶつかる敵との逆ベクトルをAddPositionする
					MelLib::Vector3 moveVector = -1 * (jEnemyPosition - iEnemyPosition);
					moveVector = moveVector.Normalize();

					const float SPEED = 0.3f;
					followEnemies[i].get()->AddPosition(moveVector * SPEED);
					followEnemies[i].get()->SetMoveCancel(true);
				}
				else
				{
					followEnemies[i].get()->SetMoveCancel(false);
				}

			}
		}
	}

}

void EnemyManager::CalcBarrierEnemyMove()
{
	for (int i = 0; i < barrierEnemies.size(); i++)
	{
		for (int j = 0; j < barrierEnemies.size(); j++)
		{
			if (i != j)
			{				
				// posをとってきて距離を計算
				MelLib::Vector3 iEnemyPosition = barrierEnemies[i].get()->GetPosition();
				MelLib::Vector3 jEnemyPosition = barrierEnemies[j].get()->GetPosition();

				// 距離を計算
				float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
				float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
				float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
				{
					// ぶつかる敵との逆ベクトルをAddPositionする
					MelLib::Vector3 moveVector = -1 * (jEnemyPosition - iEnemyPosition);
					moveVector = moveVector.Normalize();

					const float SPEED = 0.2f;
					barrierEnemies[i].get()->AddPosition(moveVector * SPEED);

					barrierEnemies[i].get()->SetMoveCancel(true);

				}
				else
				{
					barrierEnemies[i].get()->SetMoveCancel(false);
				}
			}
		}
	}
}

void EnemyManager::CalcFollowToBarrierEnemyMove()
{
	for (int i = 0; i < barrierEnemies.size(); i++)
	{
		for (int j = 0; j < followEnemies.size(); j++)
		{
			// posをとってきて距離を計算
			MelLib::Vector3 iEnemyPosition = barrierEnemies[i].get()->GetPosition();
			MelLib::Vector3 jEnemyPosition = followEnemies[j].get()->GetPosition();

			// 距離を計算
			float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
			float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
			float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			if (distance <= EnemyManage::NOT_MOVE_DISTANCE)
			{
				// ぶつかる敵との逆ベクトルをAddPositionする
				MelLib::Vector3 moveVectorBarrier = -1 * (jEnemyPosition - iEnemyPosition);
				moveVectorBarrier = moveVectorBarrier.Normalize();

				MelLib::Vector3 moveVectorFollow = -1 * (iEnemyPosition - jEnemyPosition);
			 	moveVectorFollow = moveVectorFollow.Normalize();

				const float SPEED_FOLLOW = 0.3f;
				const float SPEED_BARRIER = 0.1f;

				barrierEnemies[i].get()->AddPosition(moveVectorBarrier * SPEED_BARRIER);
				followEnemies[j].get()->AddPosition(moveVectorFollow * SPEED_FOLLOW);
				barrierEnemies[i].get()->SetMoveCancel(true);
				followEnemies[j].get()->SetMoveCancel(true);
			}
			else
			{
				barrierEnemies[i].get()->SetMoveCancel(false);
				followEnemies[j].get()->SetMoveCancel(false);
			}
		}
	}

}
