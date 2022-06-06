#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"
#include"../EnemyBarrier.h"

#include"../GameManager.h"

#include"../MyLibrary/GameObjectManager.h"

namespace EnemyManage
{
	// 最初にスポーンする追従する敵の数
	const float SPAWN_FOLLOW_ENEMY_NUM = 5;
	// 最初にスポーンするバリア持ちの敵の数
	const float SPAWN_BARRIER_ENEMY_NUM = 2;
	// スポーンする間隔
	const float SPAWN_TIME = 2000.0f;

	// 敵同士でぶつからない距離
	const float NOT_MOVE_DISTANCE = 8.0f;

	// 敵がプレイヤーからどれくらいの距離で出現しなくなるか
	const float NOT_SPAWN_DISTANCE = 20.0f;
}

// シングルトンパターン
class EnemyManager
{
private:

	static EnemyManager* instance;

	// 追いかける敵の配列
	std::vector<std::shared_ptr<FollowEnemy>> followEnemies;
	// バリア持ちの敵の配列
	std::vector <std::shared_ptr<BarrierEnemy>> barrierEnemies;
	// バリア持ちの敵用バリア配列
	std::vector<std::shared_ptr<EnemyBarrier>> enemyBarriers;

	// 時間計測用
	clock_t timerStart;

	// 出現している敵の総数
	int enemyCount;

	//敵が倒されたフレームか
	bool isDeadFlame = false;

	// プレイヤーの周辺で敵がわかない用
	MelLib::Vector3 playerPos;

	// 出現時のランダム用
	static std::random_device random;
	static std::default_random_engine engine;


public:

	static EnemyManager* GetInstance();

	// 初期配置用
	void Initialize();
	void Update();
	
	void Destroy();

	// セッター

	void SetPlayerPos(const MelLib::Vector3& pos);

	//敵が倒されたフレームかのフラグセット
	void SetIsDeadFlame(bool isDeadFlame) { this->isDeadFlame = isDeadFlame; }

	// ゲッター

	// 敵の総数を返す
	int GetEnemyCount() const;

	//敵が倒されたフレームか返す
	bool GetIsDeadFlame() { return isDeadFlame; }

private:

	EnemyManager() = default;
	~EnemyManager() = default;

	// 代入、コピーの禁止
	void operator=(const EnemyManager& obj) = delete;
	EnemyManager(const EnemyManager& obj) = delete;

	// 追従する敵の出現
	void PopFollowEnemyInitialize();

	// バリアの敵の出現
	void PopBarrierEnemyInitialize();
	// 時間での敵の出現
	void PopEnemyTime();

	// 敵が死んでいるか確認する
	template<typename T>
	void CheckEnemyDead(T& temp,bool isBarrier);

	// 敵同士でぶつかる距離まで移動しないように
	void EnemyBoidsMove();
	void CalcFollowEnemyMove();
	void CalcBarrierEnemyMove();
	void CalcFollowToBarrierEnemyMove();

	void MoveFollowEnemy();
	void MoveBarrierEnemy();
};


template<typename T>
inline void EnemyManager::CheckEnemyDead(T& temp,bool isBarrier)
{
	bool enemyDead = false;

	// 追従する敵が死んでいるのか確認
	for (auto x : temp)
	{
		// hpが0なら消す
		if (x.get()->GetHP() <= 0)
		{
			x.reset();
			enemyDead = true;

			enemyCount--;

			//ボールに敵が死んだことを伝える為のフラグ
			isDeadFlame = true;

			GameManager::GetInstance()->SetHitStop(true);
		}
	}

	// 
	if (enemyDead)
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i].get()->GetHP() <= 0)
			{
				temp.erase(temp.begin() + i);

				if (isBarrier)
				{
					enemyBarriers[i].get()->TrueEraseManager();
					enemyBarriers.erase(enemyBarriers.begin() + i);
				}
			}
		}
	}

}
