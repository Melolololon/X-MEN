#pragma once
#include<memory>
#include<time.h>

#include"../Enemy/Enemy.h"
#include"../Enemy//FollowEnemy.h"
#include"../Enemy/BarrierEnemy.h"
#include"../EnemyBarrier.h"

#include"../GameManager.h"

namespace EnemyManage
{
	// 最初にスポーンする追従する敵の数
	const float SPAWN_FOLLOW_ENEMY_NUM = 5;
	// 最初にスポーンするバリア持ちの敵の数
	const float SPAWN_BARRIER_ENEMY_NUM = 1;
	// スポーンする間隔
	const float SPAWN_TIME = 2000.0f;
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

public:

	static EnemyManager* GetInstance();

	// 初期配置用
	void Initialize();
	void Update();
	
	void Destroy();

	// セッター

	void SetPlayerPos(const MelLib::Vector3& pos);

	// ゲッター

	// 敵の総数を返す
	int GetEnemyCount() const;

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
	void CheckEnemyDead(T& temp);
};

template<typename T>
inline void EnemyManager::CheckEnemyDead(T& temp)
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
			}
		}
	}

}
