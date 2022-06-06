#include "EnemyManager.h"
#include<GameObjectManager.h>
#include<memory>
#include<time.h>
#include<random>

// 静的な実体
EnemyManager* EnemyManager::instance;
std::random_device EnemyManager::random;
std::default_random_engine EnemyManager::engine;


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
	EnemyBoidsMove();

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

	playerPos = pos;
}

int EnemyManager::GetEnemyCount() const
{
	return enemyCount;
}

void EnemyManager::PopFollowEnemyInitialize()
{
	std::uniform_int_distribution<int> dist(-20, 20);


	for (int i = 0; i < EnemyManage::SPAWN_FOLLOW_ENEMY_NUM; i++)
	{

		std::shared_ptr<FollowEnemy> temp = std::make_shared<FollowEnemy>();
		// 見た目の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(temp);

		MelLib::Vector3 spawnPos = { (float)dist(engine),0,(float)dist(engine) };

		// 敵が出現範囲外の時に再度配置
		while (true)
		{
			// 後で位置のランダム配置


			//float calcDistanceX = iEnemyPosition.x - jEnemyPosition.x;
			//float calcDistanceZ = iEnemyPosition.z - jEnemyPosition.z;
			//float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			// プレイヤーとの距離を測定
			const float calcDistanceX = playerPos.x - spawnPos.x;
			const float calcDistanceZ = playerPos.z - spawnPos.z;


			float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			//　出現禁止範囲内なら再度位置を決める
			if (distance <= EnemyManage::NOT_SPAWN_DISTANCE)
			{
				spawnPos = { (float)dist(engine),0,(float)dist(engine) };
			}
			else break;
		}


		temp.get()->SetPosition(spawnPos);
		followEnemies.push_back(temp);

		enemyCount++;
		
	}
}

void EnemyManager::PopBarrierEnemyInitialize()
{

	std::uniform_int_distribution<int> dist(-20, 20);

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


		MelLib::Vector3 spawnPos = { (float)dist(engine),0,(float)dist(engine) };
		// 敵が出現範囲外の時に再度配置
		while (true)
		{
			// プレイヤーとの距離を測定
			const float calcDistanceX = playerPos.x - spawnPos.x;
			const float calcDistanceZ = playerPos.z - spawnPos.z;


			float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			//　出現禁止範囲内なら再度位置を決める
			if (distance <= EnemyManage::NOT_SPAWN_DISTANCE)
			{
				spawnPos = { (float)dist(engine),0,(float)dist(engine) };
			}
			else break;
		}


		// とりあえず0,0,0に設置
		temp.get()->SetPosition(spawnPos);

		barrierEnemies.push_back(temp);
		temp.get()->SetBarrier(tempBarrier);
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


		std::uniform_int_distribution<int> dist(-20, 20);

		MelLib::Vector3 spawnPos = { (float)dist(engine),0,(float)dist(engine) };
		// 敵が出現範囲外の時に再度配置
		while (true)
		{
			// プレイヤーとの距離を測定
			const float calcDistanceX = playerPos.x - spawnPos.x;
			const float calcDistanceZ = playerPos.z - spawnPos.z;


			float distance = sqrt(calcDistanceX * calcDistanceX + calcDistanceZ * calcDistanceZ);

			//　出現禁止範囲内なら再度位置を決める
			if (distance <= EnemyManage::NOT_SPAWN_DISTANCE)
			{
				spawnPos = { (float)dist(engine),0,(float)dist(engine) };
			}
			else break;
		}

		followEnemy.get()->SetPosition(spawnPos);


		followEnemies.push_back(followEnemy);
		enemyCount++;

		timerStart = clock();

	}

	
}

void EnemyManager::EnemyBoidsMove()
{
	// 追従する敵の計算
	CalcFollowEnemyMove();

	// バリア持ちの計算
	CalcBarrierEnemyMove();

	// バリアと追従する敵の計算
	CalcFollowToBarrierEnemyMove();

	// 敵の移動
	MoveFollowEnemy();
	MoveBarrierEnemy();
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


				MelLib::Vector3 leaveVector = { 0,0,0 };
				//const float SPEED = 0.05f;

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
				{
					// ぶつかる敵との逆ベクトルをAddPositionする
					leaveVector = -1 * (jEnemyPosition - iEnemyPosition);
					leaveVector = leaveVector.Normalize();

				}

				// 離れるvectorをset
				followEnemies[i].get()->AddLeaveVector(leaveVector);
				

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

				MelLib::Vector3 leaveVector = { 0,0,0 };
				//const float SPEED = 0.2f;

				if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
				{
					// ぶつかる敵との逆ベクトルをAddPositionする
					leaveVector = -1 * (jEnemyPosition - iEnemyPosition);
					leaveVector = leaveVector.Normalize();


				}

				// 離れるvectorをset
				barrierEnemies[i].get()->AddLeaveVector(leaveVector);
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

			MelLib::Vector3 barrierLeaveVector = { 0,0,0 };
			MelLib::Vector3 followLeaveVector = { 0,0,0 };

			if (distance <= EnemyManage::NOT_MOVE_DISTANCE && distance > 0)
			{
				// ぶつかる敵との逆ベクトルをAddPositionする
				barrierLeaveVector = -1 * (jEnemyPosition - iEnemyPosition);
				barrierLeaveVector = barrierLeaveVector.Normalize();

				followLeaveVector = -1 * (iEnemyPosition - jEnemyPosition);
				followLeaveVector = followLeaveVector.Normalize();


			}

			barrierEnemies[i].get()->AddLeaveVector(barrierLeaveVector);
			followEnemies[j].get()->AddLeaveVector(followLeaveVector);
		}
	}

}

void EnemyManager::MoveFollowEnemy()
{
	for (auto x : followEnemies)
	{
		// 通常移動と敵との反発移動の移動量を合計して正規化する
		MelLib::Vector3 tempLeave = x.get()->GetLeaveVector().Normalize();

		MelLib::Vector3 moveVector = tempLeave + x.get()->GetMovedVector().Normalize();


		x.get()->AddPosition(moveVector * FollowEnemyStatus::FOLLOW_SPEED
			* GameManager::GetInstance()->GetGameTime());


		// 移動したらLeaveVectorの中身を初期化
		x.get()->SetLeaveVector({ 0,0,0 });

	}
}

void EnemyManager::MoveBarrierEnemy()
{
	for (auto x : barrierEnemies)
	{
		// 通常移動と敵との反発移動の移動量を合計して正規化する
		MelLib::Vector3 moveVector = x.get()->GetLeaveVector().Normalize() + x.get()->GetMovedVector();

		x.get()->AddPosition(moveVector * BarrierEnemyStatus::MOVE_SPEED * GameManager::GetInstance()->GetGameTime());


		// 移動したらLeaveVectorの中身を初期化
		x.get()->SetLeaveVector({ 0,0,0 });

	}
}
